// experiments.cpp : Defines the entry point for the console application.
//

#include <stdafx.h>
#include <cstdio>
#include <iostream>

using namespace std;

typedef void (*Foo)();

void PrintFoo() { cout << " Foo"; }

struct Bar {
  Foo foo;

  Bar(Foo test) : foo(test) { foo(); }
};

int main() {
  printf("\n\n\nWhyFoo is 0x%p\n\nPrinting: ", &PrintFoo);

  Bar bar(PrintFoo);

  printf("\nfoo is 0x%p\n\n", bar.foo);

  system("PAUSE");

  return 0;
}
