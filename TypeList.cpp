#include <string>
#include <iostream>
#include <typeinfo>
#include <type_traits>

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

template<class TList, class T> struct IndexOf;
template<class T> struct IndexOf<NullType, T>
{
   enum { value = -1 };
};

template<class Tail, class Type> struct IndexOf<TypeList<Type,Tail>, Type>
{
   enum { value = 0 };
};

template<class Head, class Tail, class Type> struct IndexOf<TypeList<Head,Tail>, Type>
{
private:
   enum { temp = IndexOf<Tail, Type>::value };
public:
   enum { value = ((temp == -1) ? -1 : temp + 1) };
};

template<class TList, class T> struct Append;
template<> struct Append<NullType, NullType>
{
   typedef NullType Result;
};

template<class T> struct Append<NullType, T>
{
   typedef TypeList<T, NullType> Result;
};

template<class Head, class Tail> struct Append<NullType, TypeList<Head, Tail>>
{
   typedef TypeList<Head, Tail> Result;
};

template<class Head, class Tail, class T> struct Append<TypeList<Head,Tail>, T>
{
   typedef TypeList<Head, typename Append<Tail,T>::Result> Result;
};

template<class TList, class T> struct EraseType;

template<class TList> struct EraseType<TList, NullType>
{
   typedef TList Result;
};

template<class Head, class Tail> struct EraseType<TypeList<Head,Tail>, Head>
{
   typedef TypeList<typename Tail::Head, typename Tail::Tail> Result;
};

template<class Head, class Tail, class T> struct EraseType<TypeList<Head,Tail>, T>
{
   typedef TypeList<Head, typename EraseType<Tail, T>::Result> Result;
};

template<class TypeListT> 
void PrintTypeListProperties()
{
   std::cout << "================== Print New Type ================== \n";
   std::cout << Length<TypeListT>::value << "\n";
   std::cout << typeid(typename TypeAt<TypeListT,0>::Result).name() << "\n";
   std::cout << IndexOf<NullType, int>::value << "\n";
   std::cout << IndexOf<TypeListT, std::string>::value << "\n";
   std::cout << "================== New Type Printed ==================  \n";
};

template<class NullTypeCandidade> struct IsNullType
{
   enum { value = false };
};

template<> struct IsNullType<NullType>
{
   enum { value = true };
};

int main()
{
   typedef TypeList<unsigned long, TypeList<std::string,NullType> > DummyTypeList;
   PrintTypeListProperties<DummyTypeList>();
   std::cout << "Is Nulltype: " << IsNullType<Append<NullType, NullType>::Result>::value << "\n";
   std::cout << "Is Nulltype: " << IsNullType<Append<NullType, int>::Result>::value << "\n";
   PrintTypeListProperties<Append<NullType, int>::Result>();
   PrintTypeListProperties<Append<NullType, DummyTypeList>::Result>();
   PrintTypeListProperties<Append<DummyTypeList,char>::Result>();
   std::cout << typeid(typename TypeAt<Append<DummyTypeList,char>::Result,2>::Result).name() << "\n";
   //PrintTypeListProperties<typename EraseType<DummyTypeList,NullType>::Result>();
   PrintTypeListProperties<typename EraseType<DummyTypeList,unsigned long>::Result>();

   return 0;
}
