#include <string>
#include <iostream>
#include <typeinfo>

struct NullType {};

template<class T , class U>
struct TypeList
{
   typedef T Head;
   typedef U Tail;
};

template<class TList> struct Length;
template<> struct Length<NullType>
{
   enum { value = 0 };
};

template<class T, class U>
struct Length< TypeList<T,U> >
{
   enum { value = 1 + Length<U>::value };
};

template<class TList, unsigned int index> struct TypeAt;
template<class T, class U> struct TypeAt<TypeList<T,U>, 0>
{
   typedef T Result;
};

template<class T, class U, unsigned int index> struct TypeAt<TypeList<T,U>,index>
{
   typedef typename TypeAt<U,index-1>::Result Result;
};

int main()
{
   typedef TypeList<unsigned long, TypeList<std::string,NullType> > TypeListT;
   std::cout << Length<TypeListT>::value << "\n";
   std::cout << typeid(typename TypeAt<TypeListT,0>::Result).name() << "\n";
   std::cout << typeid(typename TypeAt<TypeListT,1>::Result).name() << "\n";
   std::cout << typeid(typename TypeAt<TypeListT,2>::Result).name() << "\n";

   return 0;
}
