#include <fstream>
#include <iostream>
#include "text_streamwriter.hpp"
#include "text_streamreader.hpp"

using namespace std;

struct Base
{
  Base(int _b): b(_b) { }
  int b;
  virtual ~Base() { }
};

template <class T>
struct Derived: public Base
{
  T x;
  Derived(): Base(31553), x{} { }
  Derived(T _x): Base(124), x(_x) { }
  virtual ~Derived() { }
};

template <class Writer>
void serialize(Writer & w, const Base & b)
{
  w<<b.b;
}

template <class Reader>
void deserialize(Reader & r, Base & b)
{
  r>>b.b;
}

template <class Writer, class T>
void serialize(Writer & w, const Derived<T> & d)
{
  w<<static_cast<const Base>(d);
  w<<d.x;
};

template <class Reader, class T>
void deserialize(Reader & r, Derived<T> & d)
{
  r>>*static_cast<Base*>(&d);
  r>>d.x;
}

int main()
{
  Base* b1 = new Derived<float>{42.51};
  Base* b2 = new Derived<string>{"hello!"};

  ofstream stream("out.txt");
  TextStreamWriter w(stream);
  REGISTER_TYPE(w, Derived<float>);
  REGISTER_TYPE(w, Derived<string>);
  
  w<<b1;			// w<<*(Derived<float>)*(b1)
  w<<b2;

  Base* array_b[] = {b1,b2,b1,b1,b2};
  w<<array_b;
  int matrix[][3] = {{1,2,100}, {3,4,101}, {5,6,102}};
  w<<matrix;
  
  stream.close();

  ifstream readstream("out.txt");
  TextStreamReader r(readstream);
  REGISTER_TYPE(r, Derived<float>);
  REGISTER_TYPE(r, Derived<string>);

  Base* br1;
  Base* br2;
  r>>br1;
  r>>br2;

  Base* read_array_b[5];
  r>>read_array_b;

  int read_matrix[3][3] = {};
  r>>read_matrix;
  return 0;
}

/*
  Derived<float> val1 val2
  Derived<string> val1 val2

  obj = new Derived<float>();
*/
