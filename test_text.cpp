#include <fstream>
#include <iostream>
#include "text_streamwriter.hpp"

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
void serialize(Writer & w, Base b)
{
  w<<b.b;
}

template <class Writer, class T>
void serialize(Writer & w, Derived<T> d)
{
  w<<d.x;
};

int main()
{
  Base* b1 = new Derived<float>{42.51};
  Base* b2 = new Derived<string>{"hello!"};

  ofstream stream("out.txt");
  TextStreamWriter w(stream);
  REGISTER_TYPE(w,Derived<float>);
  REGISTER_TYPE(w,Derived<string>);
  w<<b1;
  w<<b2;

  Base* vec_b[] = {b1,b2,b1,b1,b2};
  w<<vec_b;
  return 0;
}
