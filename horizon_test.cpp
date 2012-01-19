#include <iostream>
#include <fstream>
#include <ostream>
#include <istream>

#include <osgViewer/Viewer>
#include <osgGeo/Horizon3D>

// Write a POD structure to stream
template<typename T>
void writeBasic(std::ostream &os, const T &data)
{
  os.write((const char*) (&data), sizeof(T));
}

// Read a POD structure from stream
template<typename T>
void readBasic(std::istream &os, T &data)
{
  os.read((char*) (&data), sizeof(T));
}

template<typename T>
int toInt(T a)
{
  return a < 0 ? (int)(a - 0.5) : (int)(a + 0.5);
}

/**
  * Class that defines a stepped range. The most common
  * usage is for inline/crossline/z seismic ranges which
  * have minimum, maximum and increment
  */
template<typename T>
struct CRange
{
  CRange(T min_, T max_, T inc_) : min(min_), max(max_), inc(inc_) {}
  CRange() : min(0), max(0), inc(0) {}

  // number of samples in the range
  int number() const;

  // save and load from standard streams
  void serialize(std::ostream &os) const;
  void deserialize(std::istream &is);

  T min, max, inc;
};

template<typename T>
int CRange<T>::number() const
{
  return toInt(fabs((this->max - this->min)/inc)) + 1;
}


template<typename T>
void CRange<T>::serialize(std::ostream &os) const
{
  writeBasic(os, this->min);
  writeBasic(os, this->max);
  writeBasic(os, inc);
}

template<typename T>
void CRange<T>::deserialize(std::istream &is)
{
  readBasic(is, this->min);
  readBasic(is, this->max);
  readBasic(is, inc);
}

void normaliseHorzFile();

int main(int argc, char **argv)
{
    if(argc <= 1)
    {
        std::cerr << "Error: Please specify horizon file" << std::endl;
        return 1;
    }

    const char *fileName = argv[1];
    std::ifstream is(fileName);
    if(!is.is_open())
    {
        std::cerr << "Error: Cannot open file " << fileName << std::endl;
        return 1;
    }

    // read inline and crossline ranges of horizon
    CRange<float> inlRange, crlRange;
    inlRange.deserialize(is);
    crlRange.deserialize(is);

    // read undefined values
    double undef;
    readBasic(is, undef);

    int numPos = inlRange.number() * crlRange.number();
    osg::ref_ptr<osg::DoubleArray> depthValsPtr =
            new osg::DoubleArray(numPos);

    osg::DoubleArray &depthVals = *depthValsPtr;

    // read depth array
    is.read((char*) depthVals.getDataPointer(), depthVals.getTotalDataSize());

    // scale Z coordinates a little bit, only temporary
    for(int i = 0; i < numPos; ++i)
    {
        if(depthVals[i] != undef)
            depthVals[i] /= 2.0;
    }

    std::vector<osg::Vec2d> coords(3);
    coords[0] = osg::Vec2d(0, 0);
    coords[1] = osg::Vec2d(0, 1);
    coords[2] = osg::Vec2d(1.4, 0);

    osg::ref_ptr<osgGeo::Horizon3DNode> horizon3d = new osgGeo::Horizon3DNode();
    horizon3d->setCornerCoords(coords);
    horizon3d->setSize(osgGeo::Vec2i(crlRange.number(), inlRange.number()));
    horizon3d->setMaxDepth(undef);
    horizon3d->setDepthArray(depthValsPtr.get());

    osgViewer::Viewer viewer;
    viewer.setSceneData(horizon3d.get());
    return viewer.run();
}

