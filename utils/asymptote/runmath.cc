/***** Autogenerated from runmath.in; changes will be overwritten *****/

#line 1 "runtimebase.in"
/*****
 * runtimebase.in
 * Andy Hammerlindl  2009/07/28
 *
 * Common declarations needed for all code-generating .in files.
 *
 *****/


#line 1 "runmath.in"
/*****
 * runmath.in
 *
 * Runtime functions for math operations.
 *
 *****/

#line 1 "runtimebase.in"
#include "stack.h"
#include "types.h"
#include "builtin.h"
#include "entry.h"
#include "errormsg.h"
#include "array.h"
#include "triple.h"
#include "callable.h"
#include "opsymbols.h"

using vm::stack;
using vm::error;
using vm::array;
using vm::read;
using vm::callable;
using types::formal;
using types::function;
using camp::triple;

#define PRIMITIVE(name,Name,asyName) using types::prim##Name;
#include <primitives.h>
#undef PRIMITIVE

typedef double real;

void unused(void *);

namespace run {
array *copyArray(array *a);
array *copyArray2(array *a);
array *copyArray3(array *a);

double *copyTripleArray2Components(array *a, size_t &N,
                                   GCPlacement placement=NoGC);
triple *copyTripleArray2C(array *a, size_t &N,
                          GCPlacement placement=NoGC);
}

function *realRealFunction();

#define CURRENTPEN processData().currentpen

#line 12 "runmath.in"
#include <inttypes.h>

#include "mathop.h"
#include "path.h"

#ifdef __CYGWIN__
extern "C" double yn(int, double);
extern "C" double jn(int, double);
#endif

using namespace camp;

typedef array realarray;
typedef array pairarray;

using types::realArray;
using types::pairArray;

using run::integeroverflow;
using vm::frame;

const char *invalidargument="invalid argument";

extern uint32_t CLZ(uint32_t a);

inline unsigned intbits() {
  static unsigned count=0;
  if(count > 0) return count;
  while((1ULL << count) < Int_MAX)
    ++count;
  ++count;
  return count;
}

static const unsigned char BitReverseTable8[256]=
{
#define R2(n)     n,    n+2*64,    n+1*64,    n+3*64
#define R4(n) R2(n),R2(n+2*16),R2(n+1*16),R2(n+3*16)
#define R6(n) R4(n),R4(n+2*4 ),R4(n+1*4 ),R4(n+3*4 )
R6(0),R6(2),R6(1),R6(3)
};
#undef R2
#undef R4
#undef R6

unsigned long long bitreverse8(unsigned long long a)
{
  return
    (unsigned long long) BitReverseTable8[a]; 
}

unsigned long long bitreverse16(unsigned long long a)
{
  return
    ((unsigned long long) BitReverseTable8[a & 0xff] << 8) | 
    ((unsigned long long) BitReverseTable8[(a >> 8)]);
}

unsigned long long bitreverse24(unsigned long long a)
{
  return
    ((unsigned long long) BitReverseTable8[a & 0xff] << 16) | 
    ((unsigned long long) BitReverseTable8[(a >> 8) & 0xff] << 8) | 
    ((unsigned long long) BitReverseTable8[(a >> 16)]);
}

unsigned long long bitreverse32(unsigned long long a)
{
  return
    ((unsigned long long) BitReverseTable8[a & 0xff] << 24) | 
    ((unsigned long long) BitReverseTable8[(a >> 8) & 0xff] << 16) | 
    ((unsigned long long) BitReverseTable8[(a >> 16) & 0xff] << 8) |
    ((unsigned long long) BitReverseTable8[(a >> 24)]);
}

unsigned long long bitreverse40(unsigned long long a)
{
  return
    ((unsigned long long) BitReverseTable8[a & 0xff] << 32) | 
    ((unsigned long long) BitReverseTable8[(a >> 8) & 0xff] << 24) | 
    ((unsigned long long) BitReverseTable8[(a >> 16) & 0xff] << 16) |
    ((unsigned long long) BitReverseTable8[(a >> 24) & 0xff] << 8) |
    ((unsigned long long) BitReverseTable8[(a >> 32)]);
}

unsigned long long bitreverse48(unsigned long long a)
{
  return
    ((unsigned long long) BitReverseTable8[a & 0xff] << 40) | 
    ((unsigned long long) BitReverseTable8[(a >> 8) & 0xff] << 32) | 
    ((unsigned long long) BitReverseTable8[(a >> 16) & 0xff] << 24) |
    ((unsigned long long) BitReverseTable8[(a >> 24) & 0xff] << 16) |
    ((unsigned long long) BitReverseTable8[(a >> 32) & 0xff] << 8) |
    ((unsigned long long) BitReverseTable8[(a >> 40)]);
}

unsigned long long bitreverse56(unsigned long long a)
{
  return
    ((unsigned long long) BitReverseTable8[a & 0xff] << 48) | 
    ((unsigned long long) BitReverseTable8[(a >> 8) & 0xff] << 40) | 
    ((unsigned long long) BitReverseTable8[(a >> 16) & 0xff] << 32) |
    ((unsigned long long) BitReverseTable8[(a >> 24) & 0xff] << 24) |
    ((unsigned long long) BitReverseTable8[(a >> 32) & 0xff] << 16) |
    ((unsigned long long) BitReverseTable8[(a >> 40) & 0xff] << 8) | 
    ((unsigned long long) BitReverseTable8[(a >> 48)]);
}

unsigned long long bitreverse64(unsigned long long a)
{
  return
    ((unsigned long long) BitReverseTable8[a & 0xff] << 56) | 
    ((unsigned long long) BitReverseTable8[(a >> 8) & 0xff] << 48) | 
    ((unsigned long long) BitReverseTable8[(a >> 16) & 0xff] << 40) |
    ((unsigned long long) BitReverseTable8[(a >> 24) & 0xff] << 32) |
    ((unsigned long long) BitReverseTable8[(a >> 32) & 0xff] << 24) |
    ((unsigned long long) BitReverseTable8[(a >> 40) & 0xff] << 16) | 
    ((unsigned long long) BitReverseTable8[(a >> 48) & 0xff] << 8) |
    ((unsigned long long) BitReverseTable8[(a >> 56)]);
}

// From Warren, Jr., Henry S. (2013) [2002]. Hacker's Delight (2 ed.).
// Addison Wesley - Pearson Education, Inc. pp. 81-96. 
Int popcount(Int a)
{
  const uint64_t m1  = 0x5555555555555555; //binary: 0101...
  const uint64_t m2  = 0x3333333333333333; //binary: 00110011..
  const uint64_t m4  = 0x0f0f0f0f0f0f0f0f; //binary:  4 zeros,  4 ones ...
  const uint64_t h01 = 0x0101010101010101; //the sum of 256 to the power of 0,1,2,3...

// This algorithm uses 12 arithmetic operations, one of which is a multiply.
  a -= (a >> 1) & m1;         //put count of each 2 bits into those 2 bits
  a=(a & m2)+((a >> 2) & m2); //put count of each 4 bits into those 4 bits 
  a=(a+(a >> 4)) & m4;        //put count of each 8 bits into those 8 bits 
  return (a * h01) >> 56;  //returns left 8 bits of a+(a << 8)+(a << 16)+(a << 24)+... 
}

// Return the factorial of a non-negative integer using a lookup table.
Int factorial(Int n)
{
  static Int *table;
  static Int size=0;
  if(size == 0) {
    Int f=1;
    size=2;
    while(f <= Int_MAX/size)
      f *= (size++);
    table=new Int[size];
    table[0]=f=1;
    for(Int i=1; i < size; ++i) {
      f *= i;
      table[i]=f;
    }
  }
  if(n >= size) integeroverflow(0);
  return table[n];
}

static inline Int Round(double x) 
{
  return Int(x+((x >= 0) ? 0.5 : -0.5));
}

inline Int sgn(double x) 
{
  return (x > 0.0 ? 1 : (x < 0.0 ? -1 : 0));
}

static bool initializeRandom=true;

void Srand(Int seed)
{ 
  initializeRandom=false;
  const int n=256;
  static char state[n];
  initstate(intcast(seed),state,n);
}  

// Autogenerated routines:



#ifndef NOSYM
#include "runmath.symbols.h"

#endif
namespace run {
#line 193 "runmath.in"
// real ^(real x, Int y);
void gen_runmath0(stack *Stack)
{
  Int y=vm::pop<Int>(Stack);
  real x=vm::pop<real>(Stack);
#line 194 "runmath.in"
  {Stack->push<real>(pow(x,y)); return;}
}

#line 198 "runmath.in"
// pair ^(pair z, Int y);
void gen_runmath1(stack *Stack)
{
  Int y=vm::pop<Int>(Stack);
  pair z=vm::pop<pair>(Stack);
#line 199 "runmath.in"
  {Stack->push<pair>(pow(z,y)); return;}
}

#line 203 "runmath.in"
// Int quotient(Int x, Int y);
void gen_runmath2(stack *Stack)
{
  Int y=vm::pop<Int>(Stack);
  Int x=vm::pop<Int>(Stack);
#line 204 "runmath.in" 
  {Stack->push<Int>(quotient<Int>()(x,y)); return;}
}

#line 208 "runmath.in"
// Int abs(Int x);
void gen_runmath3(stack *Stack)
{
  Int x=vm::pop<Int>(Stack);
#line 209 "runmath.in" 
  {Stack->push<Int>(Abs(x)); return;}
}

#line 213 "runmath.in"
// Int sgn(real x);
void gen_runmath4(stack *Stack)
{
  real x=vm::pop<real>(Stack);
#line 214 "runmath.in" 
  {Stack->push<Int>(sgn(x)); return;}
}

#line 218 "runmath.in"
// Int rand();
void gen_runmath5(stack *Stack)
{
#line 219 "runmath.in" 
  if(initializeRandom)
    Srand(1);
  {Stack->push<Int>(random()); return;}
}

#line 225 "runmath.in"
// void srand(Int seed);
void gen_runmath6(stack *Stack)
{
  Int seed=vm::pop<Int>(Stack);
#line 226 "runmath.in" 
  Srand(seed);
}

// a random number uniformly distributed in the interval [0,1]
#line 231 "runmath.in"
// real unitrand();
void gen_runmath7(stack *Stack)
{
#line 232 "runmath.in"                         
  {Stack->push<real>(((real) random())/RANDOM_MAX); return;}
}

#line 236 "runmath.in"
// Int ceil(real x);
void gen_runmath8(stack *Stack)
{
  real x=vm::pop<real>(Stack);
#line 237 "runmath.in" 
  {Stack->push<Int>(Intcast(ceil(x))); return;}
}

#line 241 "runmath.in"
// Int floor(real x);
void gen_runmath9(stack *Stack)
{
  real x=vm::pop<real>(Stack);
#line 242 "runmath.in" 
  {Stack->push<Int>(Intcast(floor(x))); return;}
}

#line 246 "runmath.in"
// Int round(real x);
void gen_runmath10(stack *Stack)
{
  real x=vm::pop<real>(Stack);
#line 247 "runmath.in" 
  if(validInt(x)) {Stack->push<Int>(Round(x)); return;}
  integeroverflow(0);
}

#line 252 "runmath.in"
// Int Ceil(real x);
void gen_runmath11(stack *Stack)
{
  real x=vm::pop<real>(Stack);
#line 253 "runmath.in" 
  {Stack->push<Int>(Ceil(x)); return;}
}

#line 257 "runmath.in"
// Int Floor(real x);
void gen_runmath12(stack *Stack)
{
  real x=vm::pop<real>(Stack);
#line 258 "runmath.in" 
  {Stack->push<Int>(Floor(x)); return;}
}

#line 262 "runmath.in"
// Int Round(real x);
void gen_runmath13(stack *Stack)
{
  real x=vm::pop<real>(Stack);
#line 263 "runmath.in" 
  {Stack->push<Int>(Round(Intcap(x))); return;}
}

#line 267 "runmath.in"
// real fmod(real x, real y);
void gen_runmath14(stack *Stack)
{
  real y=vm::pop<real>(Stack);
  real x=vm::pop<real>(Stack);
#line 268 "runmath.in"
  if (y == 0.0) dividebyzero();
  {Stack->push<real>(fmod(x,y)); return;}
}

#line 273 "runmath.in"
// real atan2(real y, real x);
void gen_runmath15(stack *Stack)
{
  real x=vm::pop<real>(Stack);
  real y=vm::pop<real>(Stack);
#line 274 "runmath.in" 
  {Stack->push<real>(atan2(y,x)); return;}
}

#line 278 "runmath.in"
// real hypot(real x, real y);
void gen_runmath16(stack *Stack)
{
  real y=vm::pop<real>(Stack);
  real x=vm::pop<real>(Stack);
#line 279 "runmath.in" 
  {Stack->push<real>(hypot(x,y)); return;}
}

#line 283 "runmath.in"
// real remainder(real x, real y);
void gen_runmath17(stack *Stack)
{
  real y=vm::pop<real>(Stack);
  real x=vm::pop<real>(Stack);
#line 284 "runmath.in" 
  {Stack->push<real>(remainder(x,y)); return;}
}

#line 288 "runmath.in"
// real Jn(Int n, real x);
void gen_runmath18(stack *Stack)
{
  real x=vm::pop<real>(Stack);
  Int n=vm::pop<Int>(Stack);
#line 289 "runmath.in"
  {Stack->push<real>(jn(n,x)); return;}
}

#line 293 "runmath.in"
// real Yn(Int n, real x);
void gen_runmath19(stack *Stack)
{
  real x=vm::pop<real>(Stack);
  Int n=vm::pop<Int>(Stack);
#line 294 "runmath.in"
  {Stack->push<real>(yn(n,x)); return;}
}

#line 298 "runmath.in"
// real erf(real x);
void gen_runmath20(stack *Stack)
{
  real x=vm::pop<real>(Stack);
#line 299 "runmath.in"
  {Stack->push<real>(erf(x)); return;}
}

#line 303 "runmath.in"
// real erfc(real x);
void gen_runmath21(stack *Stack)
{
  real x=vm::pop<real>(Stack);
#line 304 "runmath.in"
  {Stack->push<real>(erfc(x)); return;}
}

#line 308 "runmath.in"
// Int factorial(Int n);
void gen_runmath22(stack *Stack)
{
  Int n=vm::pop<Int>(Stack);
#line 309 "runmath.in"
  if(n < 0) error(invalidargument);
  {Stack->push<Int>(factorial(n)); return;}
}

#line 313 "runmath.in"
// Int choose(Int n, Int k);
void gen_runmath23(stack *Stack)
{
  Int k=vm::pop<Int>(Stack);
  Int n=vm::pop<Int>(Stack);
#line 314 "runmath.in"
  if(n < 0 || k < 0 || k > n) error(invalidargument);
  Int f=1;
  Int r=n-k;
  for(Int i=n; i > r; --i) {
    if(f > Int_MAX/i) integeroverflow(0);
    f=(f*i)/(n-i+1);
  }
  {Stack->push<Int>(f); return;}
}

#line 324 "runmath.in"
// real gamma(real x);
void gen_runmath24(stack *Stack)
{
  real x=vm::pop<real>(Stack);
#line 325 "runmath.in"
#ifdef HAVE_TGAMMA
  {Stack->push<real>(tgamma(x)); return;}
#else
  real lg = lgamma(x);
  {Stack->push<real>(signgam*exp(lg)); return;}
#endif
}

#line 334 "runmath.in"
// realarray* quadraticroots(real a, real b, real c);
void gen_runmath25(stack *Stack)
{
  real c=vm::pop<real>(Stack);
  real b=vm::pop<real>(Stack);
  real a=vm::pop<real>(Stack);
#line 335 "runmath.in"
  quadraticroots q(a,b,c);
  array *roots=new array(q.roots);
  if(q.roots >= 1) (*roots)[0]=q.t1;
  if(q.roots == 2) (*roots)[1]=q.t2;
  {Stack->push<realarray*>(roots); return;}
}

#line 343 "runmath.in"
// pairarray* quadraticroots(explicit pair a, explicit pair b, explicit pair c);
void gen_runmath26(stack *Stack)
{
  pair c=vm::pop<pair>(Stack);
  pair b=vm::pop<pair>(Stack);
  pair a=vm::pop<pair>(Stack);
#line 344 "runmath.in"
  Quadraticroots q(a,b,c);
  array *roots=new array(q.roots);
  if(q.roots >= 1) (*roots)[0]=q.z1;
  if(q.roots == 2) (*roots)[1]=q.z2;
  {Stack->push<pairarray*>(roots); return;}
}

#line 352 "runmath.in"
// realarray* cubicroots(real a, real b, real c, real d);
void gen_runmath27(stack *Stack)
{
  real d=vm::pop<real>(Stack);
  real c=vm::pop<real>(Stack);
  real b=vm::pop<real>(Stack);
  real a=vm::pop<real>(Stack);
#line 353 "runmath.in"
  cubicroots q(a,b,c,d);
  array *roots=new array(q.roots);
  if(q.roots >= 1) (*roots)[0]=q.t1;
  if(q.roots >= 2) (*roots)[1]=q.t2;
  if(q.roots == 3) (*roots)[2]=q.t3;
  {Stack->push<realarray*>(roots); return;}
}


// Logical operations
#line 364 "runmath.in"
// bool !(bool b);
void gen_runmath28(stack *Stack)
{
  bool b=vm::pop<bool>(Stack);
#line 365 "runmath.in"
  {Stack->push<bool>(!b); return;}
}

#line 370 "runmath.in"
void boolMemEq(stack *Stack)
{
  frame * b=vm::pop<frame *>(Stack);
  frame * a=vm::pop<frame *>(Stack);
#line 371 "runmath.in"
  {Stack->push<bool>(a == b); return;}
}

#line 375 "runmath.in"
void boolMemNeq(stack *Stack)
{
  frame * b=vm::pop<frame *>(Stack);
  frame * a=vm::pop<frame *>(Stack);
#line 376 "runmath.in"
  {Stack->push<bool>(a != b); return;}
}

#line 380 "runmath.in"
void boolFuncEq(stack *Stack)
{
  callable * b=vm::pop<callable *>(Stack);
  callable * a=vm::pop<callable *>(Stack);
#line 381 "runmath.in"
  {Stack->push<bool>(a->compare(b)); return;}
}

#line 385 "runmath.in"
void boolFuncNeq(stack *Stack)
{
  callable * b=vm::pop<callable *>(Stack);
  callable * a=vm::pop<callable *>(Stack);
#line 386 "runmath.in"
  {Stack->push<bool>(!(a->compare(b))); return;}
}


// Bit operations
#line 392 "runmath.in"
// Int AND(Int a, Int b);
void gen_runmath33(stack *Stack)
{
  Int b=vm::pop<Int>(Stack);
  Int a=vm::pop<Int>(Stack);
#line 393 "runmath.in"
  {Stack->push<Int>(a & b); return;}
}

#line 398 "runmath.in"
// Int OR(Int a, Int b);
void gen_runmath34(stack *Stack)
{
  Int b=vm::pop<Int>(Stack);
  Int a=vm::pop<Int>(Stack);
#line 399 "runmath.in"
  {Stack->push<Int>(a | b); return;}
}

#line 403 "runmath.in"
// Int XOR(Int a, Int b);
void gen_runmath35(stack *Stack)
{
  Int b=vm::pop<Int>(Stack);
  Int a=vm::pop<Int>(Stack);
#line 404 "runmath.in"
  {Stack->push<Int>(a ^ b); return;}
}

#line 408 "runmath.in"
// Int NOT(Int a);
void gen_runmath36(stack *Stack)
{
  Int a=vm::pop<Int>(Stack);
#line 409 "runmath.in"
  {Stack->push<Int>(~a); return;}
}

#line 413 "runmath.in"
// Int CLZ(Int a);
void gen_runmath37(stack *Stack)
{
  Int a=vm::pop<Int>(Stack);
#line 414 "runmath.in"
  if((unsigned long long) a > 0xFFFFFFFF)
    {Stack->push<Int>(CLZ((uint32_t) (a >> 32))); return;}
  else {
    int bits=intbits();
    if(a != 0) {Stack->push<Int>(bits-32+CLZ((uint32_t) a)); return;}
    {Stack->push<Int>(bits); return;}
  }
}

#line 424 "runmath.in"
// Int popcount(Int a);
void gen_runmath38(stack *Stack)
{
  Int a=vm::pop<Int>(Stack);
#line 425 "runmath.in"
  {Stack->push<Int>(popcount(a)); return;}
}

#line 429 "runmath.in"
// Int CTZ(Int a);
void gen_runmath39(stack *Stack)
{
  Int a=vm::pop<Int>(Stack);
#line 430 "runmath.in"
  {Stack->push<Int>(popcount((a&-a)-1)); return;}
}

// bitreverse a within a word of length bits.
#line 435 "runmath.in"
// Int bitreverse(Int a, Int bits);
void gen_runmath40(stack *Stack)
{
  Int bits=vm::pop<Int>(Stack);
  Int a=vm::pop<Int>(Stack);
#line 436 "runmath.in"
  typedef unsigned long long Bitreverse(unsigned long long a);
  static Bitreverse *B[]={bitreverse8,bitreverse16,bitreverse24,bitreverse32,
                     bitreverse40,bitreverse48,bitreverse56,bitreverse64};
  int maxbits=intbits()-1; // Drop sign bit
#if Int_MAX2 >= 0x7fffffffffffffffLL
  --maxbits;               // Drop extra bit for reserved values
#endif  
  if(bits <= 0 || bits > maxbits || a < 0 ||
     (unsigned long long) a >= (1ULL << bits))
    {Stack->push<Int>(-1); return;}
  unsigned int bytes=(bits+7)/8;
  {Stack->push<Int>(B[bytes-1]((unsigned long long) a) >> (8*bytes-bits)); return;}
}

} // namespace run

namespace trans {

void gen_runmath_venv(venv &ve)
{
#line 193 "runmath.in"
  addFunc(ve, run::gen_runmath0, primReal(), SYM_CARET, formal(primReal(), SYM(x), false, false), formal(primInt(), SYM(y), false, false));
#line 198 "runmath.in"
  addFunc(ve, run::gen_runmath1, primPair(), SYM_CARET, formal(primPair(), SYM(z), false, false), formal(primInt(), SYM(y), false, false));
#line 203 "runmath.in"
  addFunc(ve, run::gen_runmath2, primInt(), SYM(quotient), formal(primInt(), SYM(x), false, false), formal(primInt(), SYM(y), false, false));
#line 208 "runmath.in"
  addFunc(ve, run::gen_runmath3, primInt(), SYM(abs), formal(primInt(), SYM(x), false, false));
#line 213 "runmath.in"
  addFunc(ve, run::gen_runmath4, primInt(), SYM(sgn), formal(primReal(), SYM(x), false, false));
#line 218 "runmath.in"
  addFunc(ve, run::gen_runmath5, primInt(), SYM(rand));
#line 225 "runmath.in"
  addFunc(ve, run::gen_runmath6, primVoid(), SYM(srand), formal(primInt(), SYM(seed), false, false));
#line 230 "runmath.in"
  addFunc(ve, run::gen_runmath7, primReal(), SYM(unitrand));
#line 236 "runmath.in"
  addFunc(ve, run::gen_runmath8, primInt(), SYM(ceil), formal(primReal(), SYM(x), false, false));
#line 241 "runmath.in"
  addFunc(ve, run::gen_runmath9, primInt(), SYM(floor), formal(primReal(), SYM(x), false, false));
#line 246 "runmath.in"
  addFunc(ve, run::gen_runmath10, primInt(), SYM(round), formal(primReal(), SYM(x), false, false));
#line 252 "runmath.in"
  addFunc(ve, run::gen_runmath11, primInt(), SYM(Ceil), formal(primReal(), SYM(x), false, false));
#line 257 "runmath.in"
  addFunc(ve, run::gen_runmath12, primInt(), SYM(Floor), formal(primReal(), SYM(x), false, false));
#line 262 "runmath.in"
  addFunc(ve, run::gen_runmath13, primInt(), SYM(Round), formal(primReal(), SYM(x), false, false));
#line 267 "runmath.in"
  addFunc(ve, run::gen_runmath14, primReal(), SYM(fmod), formal(primReal(), SYM(x), false, false), formal(primReal(), SYM(y), false, false));
#line 273 "runmath.in"
  addFunc(ve, run::gen_runmath15, primReal(), SYM(atan2), formal(primReal(), SYM(y), false, false), formal(primReal(), SYM(x), false, false));
#line 278 "runmath.in"
  addFunc(ve, run::gen_runmath16, primReal(), SYM(hypot), formal(primReal(), SYM(x), false, false), formal(primReal(), SYM(y), false, false));
#line 283 "runmath.in"
  addFunc(ve, run::gen_runmath17, primReal(), SYM(remainder), formal(primReal(), SYM(x), false, false), formal(primReal(), SYM(y), false, false));
#line 288 "runmath.in"
  addFunc(ve, run::gen_runmath18, primReal(), SYM(Jn), formal(primInt(), SYM(n), false, false), formal(primReal(), SYM(x), false, false));
#line 293 "runmath.in"
  addFunc(ve, run::gen_runmath19, primReal(), SYM(Yn), formal(primInt(), SYM(n), false, false), formal(primReal(), SYM(x), false, false));
#line 298 "runmath.in"
  addFunc(ve, run::gen_runmath20, primReal(), SYM(erf), formal(primReal(), SYM(x), false, false));
#line 303 "runmath.in"
  addFunc(ve, run::gen_runmath21, primReal(), SYM(erfc), formal(primReal(), SYM(x), false, false));
#line 308 "runmath.in"
  addFunc(ve, run::gen_runmath22, primInt(), SYM(factorial), formal(primInt(), SYM(n), false, false));
#line 313 "runmath.in"
  addFunc(ve, run::gen_runmath23, primInt(), SYM(choose), formal(primInt(), SYM(n), false, false), formal(primInt(), SYM(k), false, false));
#line 324 "runmath.in"
  addFunc(ve, run::gen_runmath24, primReal(), SYM(gamma), formal(primReal(), SYM(x), false, false));
#line 334 "runmath.in"
  addFunc(ve, run::gen_runmath25, realArray(), SYM(quadraticroots), formal(primReal(), SYM(a), false, false), formal(primReal(), SYM(b), false, false), formal(primReal(), SYM(c), false, false));
#line 343 "runmath.in"
  addFunc(ve, run::gen_runmath26, pairArray(), SYM(quadraticroots), formal(primPair(), SYM(a), false, true), formal(primPair(), SYM(b), false, true), formal(primPair(), SYM(c), false, true));
#line 352 "runmath.in"
  addFunc(ve, run::gen_runmath27, realArray(), SYM(cubicroots), formal(primReal(), SYM(a), false, false), formal(primReal(), SYM(b), false, false), formal(primReal(), SYM(c), false, false), formal(primReal(), SYM(d), false, false));
#line 362 "runmath.in"
  addFunc(ve, run::gen_runmath28, primBoolean(), SYM_LOGNOT, formal(primBoolean(), SYM(b), false, false));
#line 370 "runmath.in"
  REGISTER_BLTIN(run::boolMemEq,"boolMemEq");
#line 375 "runmath.in"
  REGISTER_BLTIN(run::boolMemNeq,"boolMemNeq");
#line 380 "runmath.in"
  REGISTER_BLTIN(run::boolFuncEq,"boolFuncEq");
#line 385 "runmath.in"
  REGISTER_BLTIN(run::boolFuncNeq,"boolFuncNeq");
#line 390 "runmath.in"
  addFunc(ve, run::gen_runmath33, primInt(), SYM(AND), formal(primInt(), SYM(a), false, false), formal(primInt(), SYM(b), false, false));
#line 398 "runmath.in"
  addFunc(ve, run::gen_runmath34, primInt(), SYM(OR), formal(primInt(), SYM(a), false, false), formal(primInt(), SYM(b), false, false));
#line 403 "runmath.in"
  addFunc(ve, run::gen_runmath35, primInt(), SYM(XOR), formal(primInt(), SYM(a), false, false), formal(primInt(), SYM(b), false, false));
#line 408 "runmath.in"
  addFunc(ve, run::gen_runmath36, primInt(), SYM(NOT), formal(primInt(), SYM(a), false, false));
#line 413 "runmath.in"
  addFunc(ve, run::gen_runmath37, primInt(), SYM(CLZ), formal(primInt(), SYM(a), false, false));
#line 424 "runmath.in"
  addFunc(ve, run::gen_runmath38, primInt(), SYM(popcount), formal(primInt(), SYM(a), false, false));
#line 429 "runmath.in"
  addFunc(ve, run::gen_runmath39, primInt(), SYM(CTZ), formal(primInt(), SYM(a), false, false));
#line 434 "runmath.in"
  addFunc(ve, run::gen_runmath40, primInt(), SYM(bitreverse), formal(primInt(), SYM(a), false, false), formal(primInt(), SYM(bits), false, false));
}

} // namespace trans
