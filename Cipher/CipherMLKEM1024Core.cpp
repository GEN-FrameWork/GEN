/**-------------------------------------------------------------------------------------------------------------------
*
* @file       CipherMLKEM1024Core.cpp
*
* @class      CIPHERMLKEM1024CORE
* @brief      Load64 le
* @ingroup    CIPHER
*
* @copyright  EndoraSoft. All rights reserved.
*
* @cond
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
* documentation files(the "Software"), to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense, and/ or sell copies of the Software,
* and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of
* the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
* THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
* @endcond
*
* --------------------------------------------------------------------------------------------------------------------*/

#include "CipherMLKEM1024Core.h"
#include <string.h>

namespace
{
  const uint16_t Q = 3329;
  const int N = 256;
  const int K = 4;

  typedef uint16_t POLY[256];
  typedef POLY POLYVEC[4];

  static const uint16_t zetas[128] = {
    1,1729,2580,3289,2642,630,1897,848,1062,1919,193,797,2786,3260,569,1746,
    296,2447,1339,1476,3046,56,2240,1333,1426,2094,535,2882,2393,2879,1974,821,
    289,331,3253,1756,1197,2304,2277,2055,650,1977,2513,632,2865,33,1320,1915,
    2319,1435,807,452,1438,2868,1534,2402,2647,2617,1481,648,2474,3110,1227,910,
    17,2761,583,2649,1637,723,2288,1100,1409,2662,3281,233,756,2156,3015,3050,
    1703,1651,2789,1789,1847,952,1461,2687,939,2308,2437,2388,733,2337,268,641,
    1584,2298,2037,3220,375,2549,2090,1645,1063,319,2773,757,2099,561,2466,2594,
    2804,1092,403,1026,1143,2150,2775,886,1722,1212,1874,1029,2110,2935,885,2154
  };

  static const uint16_t gammas[128] = {
    17,3312,2761,568,583,2746,2649,680,1637,1692,723,2606,2288,1041,1100,2229,
    1409,1920,2662,667,3281,48,233,3096,756,2573,2156,1173,3015,314,3050,279,
    1703,1626,1651,1678,2789,540,1789,1540,1847,1482,952,2377,1461,1868,2687,642,
    939,2390,2308,1021,2437,892,2388,941,733,2596,2337,992,268,3061,641,2688,
    1584,1745,2298,1031,2037,1292,3220,109,375,2954,2549,780,2090,1239,1645,1684,
    1063,2266,319,3010,2773,556,757,2572,2099,1230,561,2768,2466,863,2594,735,
    2804,525,1092,2237,403,2926,1026,2303,1143,2186,2150,1179,2775,554,886,2443,
    1722,1607,1212,2117,1874,1455,1029,2300,2110,1219,2935,394,885,2444,2154,1175
  };

  /**-------------------------------------------------------------------------------------------------------------------
  * 
  * @fn         static uint64_t Load64LE(const uint8_t* p)
  * @brief      Load64 le
  * @ingroup    GEN
  * 
  * @param[in]  p : Pointer to p.
  * 
  * @return     uint64_t : Requested value.
  * 
  * --------------------------------------------------------------------------------------------------------------------*/
  static uint64_t Load64LE(const uint8_t* p)
  {
    uint64_t v=0; for(int i=0;i<8;i++) v |= ((uint64_t)p[i]) << (8*i); return v;
  }
  /**-------------------------------------------------------------------------------------------------------------------
  * 
  * @fn         static void Store64LE(uint8_t* p,uint64_t v)
  * @brief      Store64 le
  * @ingroup    GEN
  * 
  * @param[in]  p : Pointer to p.
  * @param[in]  v : V value.
  * 
  * --------------------------------------------------------------------------------------------------------------------*/
  static void Store64LE(uint8_t* p, uint64_t v)
  {
    for(int i=0;i<8;i++) p[i]=(uint8_t)(v>>(8*i));
  }
  /**-------------------------------------------------------------------------------------------------------------------
  * 
  * @fn         static uint64_t ROL64(uint64_t x,int n)
  * @brief      Rol64
  * @ingroup    GEN
  * 
  * @param[in]  x : X value.
  * @param[in]  n : N value.
  * 
  * @return     uint64_t : Requested value.
  * 
  * --------------------------------------------------------------------------------------------------------------------*/
  static uint64_t ROL64(uint64_t x, int n) { return (x<<n)|(x>>(64-n)); }

  /**-------------------------------------------------------------------------------------------------------------------
  * 
  * @fn         static void KeccakF(uint64_t st[25])
  * @brief      Keccak f
  * @ingroup    GEN
  * 
  * @param[in]  Value.
  * 
  * --------------------------------------------------------------------------------------------------------------------*/
  static void KeccakF(uint64_t st[25])
  {
    static const uint64_t rc[24]={
      0x0000000000000001ULL,0x0000000000008082ULL,0x800000000000808aULL,0x8000000080008000ULL,
      0x000000000000808bULL,0x0000000080000001ULL,0x8000000080008081ULL,0x8000000000008009ULL,
      0x000000000000008aULL,0x0000000000000088ULL,0x0000000080008009ULL,0x000000008000000aULL,
      0x000000008000808bULL,0x800000000000008bULL,0x8000000000008089ULL,0x8000000000008003ULL,
      0x8000000000008002ULL,0x8000000000000080ULL,0x000000000000800aULL,0x800000008000000aULL,
      0x8000000080008081ULL,0x8000000000008080ULL,0x0000000080000001ULL,0x8000000080008008ULL};
    static const int r[25]={0,1,62,28,27,36,44,6,55,20,3,10,43,25,39,41,45,15,21,8,18,2,61,56,14};
    for(int round=0;round<24;round++) {
      uint64_t c[5],d[5],b[25];
      for(int x=0;x<5;x++) c[x]=st[x]^st[x+5]^st[x+10]^st[x+15]^st[x+20];
      for(int x=0;x<5;x++) d[x]=c[(x+4)%5]^ROL64(c[(x+1)%5], 1);
      for(int y=0;y<5;y++) for(int x=0;x<5;x++) st[x+5*y]^=d[x];
      for(int y=0;y<5;y++) for(int x=0;x<5;x++) {
        int nx=y, ny=(2*x+3*y)%5;
        b[nx+5*ny]=r[x+5*y]?ROL64(st[x+5*y], r[x+5*y]):st[x+5*y];
      }
      for(int y=0;y<5;y++) for(int x=0;x<5;x++) st[x+5*y]=b[x+5*y]^((~b[(x+1)%5+5*y])&b[(x+2)%5+5*y]);
      st[0]^=rc[round];
    }
  }

  /**-------------------------------------------------------------------------------------------------------------------
  * 
  * @fn         static void Sponge(const uint8_t* in,size_t inlen,uint8_t* out,size_t outlen,size_t rate,uint8_t domain)
  * @brief      Sponge
  * @ingroup    GEN
  * 
  * @param[in]  in : Pointer to in.
  * @param[in]  inlen : Inlen value.
  * @param[out] out : Pointer to out.
  * @param[in]  outlen : Outlen value.
  * @param[in]  rate : Rate value.
  * @param[in]  domain : Domain value.
  * 
  * --------------------------------------------------------------------------------------------------------------------*/
  static void Sponge(const uint8_t* in, size_t inlen, uint8_t* out, size_t outlen, size_t rate, uint8_t domain)
  {
    uint64_t st[25]={0};
    while(inlen>=rate) {
      for(size_t i=0;i<rate/8;i++) st[i]^=Load64LE(in+8*i);
      KeccakF(st); in+=rate; inlen-=rate;
    }
    uint8_t block[200]={0};
    if(inlen) memcpy(block, in, inlen);
    block[inlen]^=domain; block[rate-1]^=0x80;
    for(size_t i=0;i<rate/8;i++) st[i]^=Load64LE(block+8*i);
    KeccakF(st);
    while(outlen) {
      size_t n=outlen<rate?outlen:rate;
      size_t full=n/8;
      for(size_t i=0;i<full;i++) Store64LE(out+8*i, st[i]);
      if(n&7) { uint8_t temp[8]; Store64LE(temp, st[full]); memcpy(out+full*8, temp, n&7); }
      out+=n; outlen-=n;
      if(outlen) KeccakF(st);
    }
    memset(st, 0, sizeof(st)); memset(block, 0, sizeof(block));
  }
  /**-------------------------------------------------------------------------------------------------------------------
  * 
  * @fn         static void SHA3_256(const uint8_t* in,size_t len,uint8_t out[32])
  * @brief      256
  * @ingroup    GEN
  * 
  * @param[in]  in : Pointer to in.
  * @param[in]  len : Len value.
  * @param[in]  Value.
  * 
  * --------------------------------------------------------------------------------------------------------------------*/
  static void SHA3_256(const uint8_t* in, size_t len, uint8_t out[32]) { Sponge(in, len, out, 32, 136, 0x06); }
  /**-------------------------------------------------------------------------------------------------------------------
  * 
  * @fn         static void SHA3_512(const uint8_t* in,size_t len,uint8_t out[64])
  * @brief      512
  * @ingroup    GEN
  * 
  * @param[in]  in : Pointer to in.
  * @param[in]  len : Len value.
  * @param[in]  Value.
  * 
  * --------------------------------------------------------------------------------------------------------------------*/
  static void SHA3_512(const uint8_t* in, size_t len, uint8_t out[64]) { Sponge(in, len, out, 64, 72, 0x06); }
  /**-------------------------------------------------------------------------------------------------------------------
  * 
  * @fn         static void SHAKE128(const uint8_t* in,size_t len,uint8_t* out,size_t outlen)
  * @brief      Shake128
  * @ingroup    GEN
  * 
  * @param[in]  in : Pointer to in.
  * @param[in]  len : Len value.
  * @param[out] out : Pointer to out.
  * @param[in]  outlen : Outlen value.
  * 
  * --------------------------------------------------------------------------------------------------------------------*/
  static void SHAKE128(const uint8_t* in, size_t len, uint8_t* out, size_t outlen) { Sponge(in, len, out, outlen, 168, 0x1f); }
  /**-------------------------------------------------------------------------------------------------------------------
  * 
  * @fn         static void SHAKE256(const uint8_t* in,size_t len,uint8_t* out,size_t outlen)
  * @brief      Shake256
  * @ingroup    GEN
  * 
  * @param[in]  in : Pointer to in.
  * @param[in]  len : Len value.
  * @param[out] out : Pointer to out.
  * @param[in]  outlen : Outlen value.
  * 
  * --------------------------------------------------------------------------------------------------------------------*/
  static void SHAKE256(const uint8_t* in, size_t len, uint8_t* out, size_t outlen) { Sponge(in, len, out, outlen, 136, 0x1f); }

  /**-------------------------------------------------------------------------------------------------------------------
  * 
  * @fn         static uint16_t Add(uint16_t a,uint16_t b)
  * @brief      Add
  * @ingroup    GEN
  * 
  * @param[in]  a : A value.
  * @param[in]  b : B value.
  * 
  * @return     uint16_t : Requested value.
  * 
  * --------------------------------------------------------------------------------------------------------------------*/
  static uint16_t Add(uint16_t a, uint16_t b) { uint16_t x=a+b; if(x>=Q)x-=Q; return x; }
  /**-------------------------------------------------------------------------------------------------------------------
  * 
  * @fn         static uint16_t Sub(uint16_t a,uint16_t b)
  * @brief      Sub
  * @ingroup    GEN
  * 
  * @param[in]  a : A value.
  * @param[in]  b : B value.
  * 
  * @return     uint16_t : Requested value.
  * 
  * --------------------------------------------------------------------------------------------------------------------*/
  static uint16_t Sub(uint16_t a, uint16_t b) { return a>=b?a-b:(uint16_t)(a+Q-b); }
  /**-------------------------------------------------------------------------------------------------------------------
  * 
  * @fn         static uint16_t Mul(uint16_t a,uint16_t b)
  * @brief      Mul
  * @ingroup    GEN
  * 
  * @param[in]  a : A value.
  * @param[in]  b : B value.
  * 
  * @return     uint16_t : Requested value.
  * 
  * --------------------------------------------------------------------------------------------------------------------*/
  static uint16_t Mul(uint16_t a, uint16_t b) { return (uint16_t)(((uint32_t)a*b)%Q); }

  /**-------------------------------------------------------------------------------------------------------------------
  * 
  * @fn         static void PolyAdd(POLY r,const POLY a,const POLY b)
  * @brief      Poly add
  * @ingroup    GEN
  * 
  * @param[in]  r : R value.
  * @param[in]  a : A value.
  * @param[in]  b : B value.
  * 
  * --------------------------------------------------------------------------------------------------------------------*/
  static void PolyAdd(POLY r, const POLY a, const POLY b) { for(int i=0;i<N;i++) r[i]=Add(a[i], b[i]); }
  /**-------------------------------------------------------------------------------------------------------------------
  * 
  * @fn         static void PolySub(POLY r,const POLY a,const POLY b)
  * @brief      Poly sub
  * @ingroup    GEN
  * 
  * @param[in]  r : R value.
  * @param[in]  a : A value.
  * @param[in]  b : B value.
  * 
  * --------------------------------------------------------------------------------------------------------------------*/
  static void PolySub(POLY r, const POLY a, const POLY b) { for(int i=0;i<N;i++) r[i]=Sub(a[i], b[i]); }

  /**-------------------------------------------------------------------------------------------------------------------
  * 
  * @fn         static void NTT(POLY f)
  * @brief      Ntt
  * @ingroup    GEN
  * 
  * @param[in]  f : F value.
  * 
  * --------------------------------------------------------------------------------------------------------------------*/
  static void NTT(POLY f)
  {
    int kk=1;
    for(int len=128;len>=2;len>>=1) for(int start=0;start<256;start+=2*len) {
      uint16_t z=zetas[kk++];
      for(int j=start;j<start+len;j++) { uint16_t t=Mul(z, f[j+len]); f[j+len]=Sub(f[j], t); f[j]=Add(f[j], t); }
    }
  }
  /**-------------------------------------------------------------------------------------------------------------------
  * 
  * @fn         static void InvNTT(POLY f)
  * @brief      Inv ntt
  * @ingroup    GEN
  * 
  * @param[in]  f : F value.
  * 
  * --------------------------------------------------------------------------------------------------------------------*/
  static void InvNTT(POLY f)
  {
    int kk=127;
    for(int len=2;len<=128;len<<=1) for(int start=0;start<256;start+=2*len) {
      uint16_t z=zetas[kk--];
      for(int j=start;j<start+len;j++) { uint16_t t=f[j]; f[j]=Add(t, f[j+len]); f[j+len]=Mul(z, Sub(f[j+len], t)); }
    }
    for(int i=0;i<N;i++) f[i]=Mul(f[i], 3303);
  }
  /**-------------------------------------------------------------------------------------------------------------------
  * 
  * @fn         static void NTTMul(POLY h,const POLY f,const POLY g)
  * @brief      Ntt mul
  * @ingroup    GEN
  * 
  * @param[in]  h : H value.
  * @param[in]  f : F value.
  * @param[in]  g : G value.
  * 
  * --------------------------------------------------------------------------------------------------------------------*/
  static void NTTMul(POLY h, const POLY f, const POLY g)
  {
    for(int i=0;i<N;i+=2) {
      uint16_t a0=f[i],a1=f[i+1],b0=g[i],b1=g[i+1],gamma=gammas[i/2];
      h[i]=Add(Mul(a0, b0), Mul(Mul(a1, b1), gamma));
      h[i+1]=Add(Mul(a0, b1), Mul(a1, b0));
    }
  }

  /**-------------------------------------------------------------------------------------------------------------------
  * 
  * @fn         static uint16_t Compress(uint16_t x,int d)
  * @brief      Compress
  * @ingroup    GEN
  * 
  * @param[in]  x : X value.
  * @param[in]  d : D value.
  * 
  * @return     uint16_t : Requested value.
  * 
  * --------------------------------------------------------------------------------------------------------------------*/
  static uint16_t Compress(uint16_t x, int d)
  {
    uint32_t dividend=(uint32_t)x<<d;
    uint32_t quotient=(uint32_t)(((uint64_t)dividend*5039)>>24);
    uint32_t remainder=dividend-quotient*Q;
    quotient += ((uint32_t)(Q/2)-remainder)>>31 & 1;
    quotient += ((uint32_t)(Q+Q/2)-remainder)>>31 & 1;
    return (uint16_t)(quotient&((1u<<d)-1));
  }
  /**-------------------------------------------------------------------------------------------------------------------
  * 
  * @fn         static uint16_t Decompress(uint16_t y,int d)
  * @brief      Decompress
  * @ingroup    GEN
  * 
  * @param[in]  y : Y value.
  * @param[in]  d : D value.
  * 
  * @return     uint16_t : Requested value.
  * 
  * --------------------------------------------------------------------------------------------------------------------*/
  static uint16_t Decompress(uint16_t y, int d)
  {
    uint32_t dividend=(uint32_t)y*Q;
    uint32_t quotient=dividend>>d;
    quotient += (dividend>>(d-1))&1;
    return (uint16_t)quotient;
  }

  /**-------------------------------------------------------------------------------------------------------------------
  * 
  * @fn         static void Encode12(uint8_t* out,const POLY f)
  * @brief      Encode12
  * @ingroup    GEN
  * 
  * @param[out] out : Pointer to out.
  * @param[in]  f : F value.
  * 
  * --------------------------------------------------------------------------------------------------------------------*/
  static void Encode12(uint8_t* out, const POLY f)
  {
    for(int i=0;i<N;i+=2) { uint32_t x=(uint32_t)f[i]|((uint32_t)f[i+1]<<12); *out++=(uint8_t)x;*out++=(uint8_t)(x>>8);*out++=(uint8_t)(x>>16); }
  }
  /**-------------------------------------------------------------------------------------------------------------------
  * 
  * @fn         static bool Decode12(POLY f,const uint8_t* in)
  * @brief      Decode12
  * @ingroup    GEN
  * 
  * @param[in]  f : F value.
  * @param[in]  in : Pointer to in.
  * 
  * @return     bool : true if the operation is successful; otherwise false.
  * 
  * --------------------------------------------------------------------------------------------------------------------*/
  static bool Decode12(POLY f, const uint8_t* in)
  {
    for(int i=0;i<N;i+=2) { uint32_t x=(uint32_t)in[0]|((uint32_t)in[1]<<8)|((uint32_t)in[2]<<16); in+=3; f[i]=(uint16_t)(x&0xfff); f[i+1]=(uint16_t)(x>>12); if(f[i]>=Q||f[i+1]>=Q)return false; }
    return true;
  }
  /**-------------------------------------------------------------------------------------------------------------------
  * 
  * @fn         static void EncodeD(uint8_t* out,const POLY f,int d,bool compress)
  * @brief      Encode d
  * @ingroup    GEN
  * 
  * @param[out] out : Pointer to out.
  * @param[in]  f : F value.
  * @param[in]  d : D value.
  * @param[in]  compress : Compress value.
  * 
  * --------------------------------------------------------------------------------------------------------------------*/
  static void EncodeD(uint8_t* out, const POLY f, int d, bool compress)
  {
    const size_t bytes=(N*d)/8; memset(out, 0, bytes); uint32_t bit=0;
    for(int i=0;i<N;i++) { uint16_t v=compress?Compress(f[i], d):f[i]; for(int j=0;j<d;j++,bit++) if(v&(1u<<j)) out[bit>>3]|=(uint8_t)(1u<<(bit&7)); }
  }
  /**-------------------------------------------------------------------------------------------------------------------
  * 
  * @fn         static void DecodeD(POLY f,const uint8_t* in,int d,bool decompress)
  * @brief      Decode d
  * @ingroup    GEN
  * 
  * @param[in]  f : F value.
  * @param[in]  in : Pointer to in.
  * @param[in]  d : D value.
  * @param[in]  decompress : Decompress value.
  * 
  * --------------------------------------------------------------------------------------------------------------------*/
  static void DecodeD(POLY f, const uint8_t* in, int d, bool decompress)
  {
    uint32_t bit=0; for(int i=0;i<N;i++) { uint16_t v=0; for(int j=0;j<d;j++,bit++) v|=(uint16_t)(((in[bit>>3]>>(bit&7))&1u)<<j); f[i]=decompress?Decompress(v, d):v; }
  }

  /**-------------------------------------------------------------------------------------------------------------------
  * 
  * @fn         static void SampleCBD(POLY f,const uint8_t sigma[32],uint8_t nonce)
  * @brief      Sample cbd
  * @ingroup    GEN
  * 
  * @param[in]  f : F value.
  * @param[in]  Value.
  * @param[in]  nonce : Nonce value.
  * 
  * --------------------------------------------------------------------------------------------------------------------*/
  static void SampleCBD(POLY f, const uint8_t sigma[32], uint8_t nonce)
  {
    uint8_t in[33],b[128]; memcpy(in, sigma, 32);in[32]=nonce; SHAKE256(in, sizeof(in), b, sizeof(b));
    for(int i=0;i<N;i++) { int bit=4*i; int a=((b[bit>>3]>>(bit&7))&1)+((b[(bit+1)>>3]>>((bit+1)&7))&1); int bb=((b[(bit+2)>>3]>>((bit+2)&7))&1)+((b[(bit+3)>>3]>>((bit+3)&7))&1); int v=a-bb; f[i]=(uint16_t)(v<0?v+Q:v); }
    memset(in, 0, sizeof(in));memset(b, 0, sizeof(b));
  }

  /**-------------------------------------------------------------------------------------------------------------------
  * 
  * @fn         static bool SampleNTT(POLY a,const uint8_t rho[32],uint8_t ii,uint8_t jj)
  * @brief      Sample ntt
  * @ingroup    GEN
  * 
  * @param[in]  a : A value.
  * @param[in]  Value.
  * @param[in]  ii : Ii value.
  * @param[in]  jj : Jj value.
  * 
  * @return     bool : true if the operation is successful; otherwise false.
  * 
  * --------------------------------------------------------------------------------------------------------------------*/
  static bool SampleNTT(POLY a, const uint8_t rho[32], uint8_t ii, uint8_t jj)
  {
    uint8_t in[34],stream[1024]; memcpy(in, rho, 32);in[32]=ii;in[33]=jj;SHAKE128(in, sizeof(in), stream, sizeof(stream));
    int n=0; size_t off=0; while(n<N && off+2<sizeof(stream)) { uint16_t d1=(uint16_t)(stream[off]|((uint16_t)(stream[off+1]&0x0f)<<8)); uint16_t d2=(uint16_t)((stream[off+1]>>4)|((uint16_t)stream[off+2]<<4)); off+=3; if(d1<Q)a[n++]=d1; if(n<N&&d2<Q)a[n++]=d2; }
    return n==N;
  }

  /**-------------------------------------------------------------------------------------------------------------------
  * 
  * @fn         static bool ParseEK(POLYVEC t,POLY A[16],const uint8_t ek[1568])
  * @brief      Parse ek
  * @ingroup    GEN
  * 
  * @param[in]  t : T value.
  * @param[in]  Value.
  * @param[in]  Value.
  * 
  * @return     bool : true if the operation is successful; otherwise false.
  * 
  * --------------------------------------------------------------------------------------------------------------------*/
  static bool ParseEK(POLYVEC t, POLY A[16], const uint8_t ek[1568])
  {
    const uint8_t* p=ek; for(int i=0;i<K;i++){ if(!Decode12(t[i], p))return false;p+=384; } const uint8_t* rho=p;
    for(int i=0;i<K;i++)for(int j=0;j<K;j++)if(!SampleNTT(A[i*K+j], rho, (uint8_t)j, (uint8_t)i))return false;
    return true;
  }

  /**-------------------------------------------------------------------------------------------------------------------
  * 
  * @fn         static void PKEEncrypt(uint8_t c[1568],const uint8_t ek[1568],const uint8_t m[32],const uint8_t rnd[32])
  * @brief      Pke encrypt
  * @ingroup    GEN
  * 
  * @param[in]  Value.
  * @param[in]  Value.
  * @param[in]  Value.
  * @param[in]  Value.
  * 
  * --------------------------------------------------------------------------------------------------------------------*/
  static void PKEEncrypt(uint8_t c[1568], const uint8_t ek[1568], const uint8_t m[32], const uint8_t rnd[32])
  {
    POLYVEC t,r,e1,u; POLY A[16],e2,mu,vntt,v,temp,prod; ParseEK(t, A, ek);
    uint8_t nonce=0; for(int i=0;i<K;i++){SampleCBD(r[i], rnd, nonce++);NTT(r[i]);} for(int i=0;i<K;i++)SampleCBD(e1[i], rnd, nonce++); SampleCBD(e2, rnd, nonce++);
    for(int i=0;i<K;i++) {
      memcpy(u[i], e1[i], sizeof(POLY));
      for(int j=0;j<K;j++){NTTMul(prod, A[j*K+i], r[j]);InvNTT(prod);PolyAdd(temp, u[i], prod);memcpy(u[i], temp, sizeof(POLY));}
    }
    DecodeD(mu, m, 1, true);
    memset(vntt, 0, sizeof(vntt));
    for(int i=0;i<K;i++){NTTMul(prod, t[i], r[i]);PolyAdd(temp, vntt, prod);memcpy(vntt, temp, sizeof(POLY));}
    memcpy(v, vntt, sizeof(POLY));InvNTT(v);PolyAdd(temp, v, e2);PolyAdd(v, temp, mu);
    for(int i=0;i<K;i++)EncodeD(c+i*352, u[i], 11, true); EncodeD(c+1408, v, 5, true);
    memset(r, 0, sizeof(r)); memset(e1, 0, sizeof(e1)); memset(e2, 0, sizeof(e2));
  }

  /**-------------------------------------------------------------------------------------------------------------------
  * 
  * @fn         static bool PKEDecrypt(uint8_t m[32],const uint8_t dkpke[1536],const uint8_t c[1568])
  * @brief      Pke decrypt
  * @ingroup    GEN
  * 
  * @param[in]  Value.
  * @param[in]  Value.
  * @param[in]  Value.
  * 
  * @return     bool : true if the operation is successful; otherwise false.
  * 
  * --------------------------------------------------------------------------------------------------------------------*/
  static bool PKEDecrypt(uint8_t m[32], const uint8_t dkpke[1536], const uint8_t c[1568])
  {
    POLYVEC s,u; POLY v,mask,prod,temp,w; const uint8_t* p=dkpke;
    for(int i=0;i<K;i++){if(!Decode12(s[i], p))return false;p+=384;}
    for(int i=0;i<K;i++)DecodeD(u[i], c+i*352, 11, true); DecodeD(v, c+1408, 5, true);
    memset(mask, 0, sizeof(mask));
    for(int i=0;i<K;i++){NTT(u[i]);NTTMul(prod, s[i], u[i]);PolyAdd(temp, mask, prod);memcpy(mask, temp, sizeof(POLY));}
    InvNTT(mask);PolySub(w, v, mask);EncodeD(m, w, 1, true);
    memset(s, 0, sizeof(s)); return true;
  }

  /**-------------------------------------------------------------------------------------------------------------------
  * 
  * @fn         static bool ConstantEqual(const uint8_t* a,const uint8_t* b,size_t n)
  * @brief      Constant equal
  * @ingroup    GEN
  * 
  * @param[in]  a : Pointer to a.
  * @param[in]  b : Pointer to b.
  * @param[in]  n : N value.
  * 
  * @return     bool : true if the operation is successful; otherwise false.
  * 
  * --------------------------------------------------------------------------------------------------------------------*/
  static bool ConstantEqual(const uint8_t* a, const uint8_t* b, size_t n)
  { uint8_t d=0;for(size_t i=0;i<n;i++)d|=a[i]^b[i];return d==0; }
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERMLKEM1024CORE::PublicKey_Check(const uint8_t publickey[1568])
* @brief      Public key check
* @ingroup    GEN
* 
* @param[in]  publickey : 
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERMLKEM1024CORE::PublicKey_Check(const uint8_t publickey[1568])
{
  if(!publickey)return false; POLY f; for(int i=0;i<4;i++)if(!Decode12(f, publickey+i*384))return false; return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERMLKEM1024CORE::KeyPair(const uint8_t d[32], const uint8_t z[32], uint8_t ek[1568], uint8_t dk[3168])
* @brief      Key pair
* @ingroup    GEN
* 
* @param[in]  d : 
* @param[in]  z : 
* @param[in]  ek : 
* @param[in]  dk : 
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERMLKEM1024CORE::KeyPair(const uint8_t d[32], const uint8_t z[32], uint8_t ek[1568], uint8_t dk[3168])
{
  if(!d||!z||!ek||!dk)return false;
  uint8_t seed[33],G[64];memcpy(seed, d, 32);seed[32]=4;SHA3_512(seed, sizeof(seed), G);const uint8_t* rho=G;const uint8_t* sigma=G+32;
  POLY A[16];POLYVEC s,e,t;uint8_t nonce=0;
  for(int i=0;i<K;i++)for(int j=0;j<K;j++)if(!SampleNTT(A[i*K+j], rho, (uint8_t)j, (uint8_t)i))return false;
  for(int i=0;i<K;i++){SampleCBD(s[i], sigma, nonce++);NTT(s[i]);} for(int i=0;i<K;i++){SampleCBD(e[i], sigma, nonce++);NTT(e[i]);}
  for(int i=0;i<K;i++){memcpy(t[i], e[i], sizeof(POLY));for(int j=0;j<K;j++){POLY prod,tmp;NTTMul(prod, A[i*K+j], s[j]);PolyAdd(tmp, t[i], prod);memcpy(t[i], tmp, sizeof(POLY));}}
  uint8_t* p=dk;for(int i=0;i<K;i++){Encode12(p, s[i]);p+=384;} uint8_t* ep=ek;for(int i=0;i<K;i++){Encode12(ep, t[i]);ep+=384;}memcpy(ep, rho, 32);
  memcpy(p, ek, 1568);p+=1568;SHA3_256(ek, 1568, p);p+=32;memcpy(p, z, 32);
  memset(seed, 0, sizeof(seed));memset(G, 0, sizeof(G));memset(s, 0, sizeof(s));memset(e, 0, sizeof(e));return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERMLKEM1024CORE::Encapsulate(const uint8_t randomness[32], const uint8_t ek[1568], uint8_t c[1568], uint8_t ss[32])
* @brief      Encapsulate
* @ingroup    GEN
* 
* @param[in]  randomness : 
* @param[in]  ek : 
* @param[in]  c : 
* @param[in]  ss : 
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERMLKEM1024CORE::Encapsulate(const uint8_t randomness[32], const uint8_t ek[1568], uint8_t c[1568], uint8_t ss[32])
{
  if(!randomness||!ek||!c||!ss||!PublicKey_Check(ek))return false;
  uint8_t h[32],input[64],g[64];SHA3_256(ek, 1568, h);memcpy(input, randomness, 32);memcpy(input+32, h, 32);SHA3_512(input, sizeof(input), g);memcpy(ss, g, 32);PKEEncrypt(c, ek, randomness, g+32);
  memset(h, 0, sizeof(h));memset(input, 0, sizeof(input));memset(g, 0, sizeof(g));return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERMLKEM1024CORE::Decapsulate(const uint8_t dk[3168], const uint8_t c[1568], uint8_t ss[32])
* @brief      Decapsulate
* @ingroup    GEN
* 
* @param[in]  dk : 
* @param[in]  c : 
* @param[in]  ss : 
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERMLKEM1024CORE::Decapsulate(const uint8_t dk[3168], const uint8_t c[1568], uint8_t ss[32])
{
  if(!dk||!c||!ss)return false;
  const uint8_t* dkpke=dk;const uint8_t* ek=dk+1536;const uint8_t* h=ek+1568;const uint8_t* z=h+32;
  uint8_t m[32],input[64],g[64],cc[1568],rejectinput[1600],reject[32];
  if(!PKEDecrypt(m, dkpke, c))return false;memcpy(input, m, 32);memcpy(input+32, h, 32);SHA3_512(input, sizeof(input), g);PKEEncrypt(cc, ek, m, g+32);
  memcpy(rejectinput, z, 32);memcpy(rejectinput+32, c, 1568);SHAKE256(rejectinput, sizeof(rejectinput), reject, 32);
  uint8_t mask=(uint8_t)-(int)ConstantEqual(c, cc, 1568);for(int i=0;i<32;i++)ss[i]=(g[i]&mask)|(reject[i]&~mask);
  memset(m, 0, sizeof(m));memset(input, 0, sizeof(input));memset(g, 0, sizeof(g));memset(cc, 0, sizeof(cc));memset(reject, 0, sizeof(reject));return true;
}
