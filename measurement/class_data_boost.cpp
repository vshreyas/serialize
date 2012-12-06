/**
 * @brief Describes serialization and deserialization of several
 * types, including classes, polymorphic objects, multidimensional
 * arrays, and vectors.
 * 
 * This uses the text version of the Boost serializer.
 *
 * Size of output file: 126 bytes
 * Lines of code saved: 5
 */

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>

#include <fstream>
#include <iostream>

using namespace std;

struct Base
{
  // required or compile-time error.
  Base(): b(0) { }
  
  Base(int _b): b(_b) { }

  virtual ~Base() { }

  int b;
  
  template <class Archive>
  void serialize(Archive &ar, const unsigned int version)
  {
    ar & b;
  }
};

template <class T>
struct Derived: public Base
{
  T x;
  Derived(): Base(31553), x{} { }
  Derived(T _x): Base(124), x(_x) { }
  virtual ~Derived() { }

  template <class Archive>
  void serialize(Archive &ar, const unsigned int version)
  {
    // save/load base class information
    ar & boost::serialization::base_object<Base>(*this);
    ar & x;
  }

};

int main()
{
  Base* b1 = new Derived<float>{42.51};
  Base* b2 = new Derived<string>{"hello!"};

  ofstream stream("serialized_class_boost.txt");
  boost::archive::text_oarchive w(stream);
  w.register_type(static_cast< Derived<float> *>(NULL));
  w.register_type(static_cast< Derived<string> *>(NULL));
    
  w<<b1;
  w<<b2;

  int matrix[][3] = {{1,2,100}, {3,4,101}, {5,6,102}};
  w<<matrix;

  vector<int> v_int = {1,23,125,21};
  w<<v_int;
  
  stream.close();

  ifstream readstream("serialized_class_boost.txt");
  boost::archive::text_iarchive r(readstream);
  r.register_type(static_cast< Derived<float> *>(NULL));
  r.register_type(static_cast< Derived<string> *>(NULL));
  
  Base* br1;
  Base* br2;
  r>>br1;
  r>>br2;

  int read_matrix[3][3] = {};
  r>>read_matrix;

  vector<int> v_read;
  r>>v_read;

  readstream.close();
  
  return 0;
}
