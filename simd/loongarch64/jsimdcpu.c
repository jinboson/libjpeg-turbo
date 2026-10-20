/*
 * Copyright (C) 2023, Loongson Technology Corporation Limited.
 *                     Author:  Song Ding <songding@loongson.cn>
 * Copyright (C) 2026, D. R. Commander.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * This file contains the interface between the "normal" portions
 * of the library and the SIMD implementations when running on a
 * 64-bit LoongArch architecture.
 */

#include "../jsimdint.h"

#if defined(HAVE_GETAUXVAL)

#include <sys/auxv.h>

#if defined(__linux__)
#include <asm/hwcap.h>
#endif

#if !defined(HWCAP_LOONGARCH_LSX)
#define HWCAP_LOONGARCH_LSX   (1 << 4)
#endif
#if !defined(HWCAP_LOONGARCH_LASX)
#define HWCAP_LOONGARCH_LASX  (1 << 5)
#endif

#endif /* HAVE_GETAUXVAL */


HIDDEN unsigned int
jpeg_simd_cpu_support(void)
{
  unsigned int simd_support = 0;

#if defined(HAVE_GETAUXVAL)
  {
    unsigned long cpufeatures = getauxval(AT_HWCAP);

    if (cpufeatures & HWCAP_LOONGARCH_LSX)
      simd_support |= JSIMD_LSX;
#if defined(JSIMD_WITH_LASX)
    if (cpufeatures & HWCAP_LOONGARCH_LASX)
      simd_support |= JSIMD_LASX;
#endif
  }
#elif defined(__loongarch_sx)
  simd_support |= JSIMD_LSX;
#if defined(__loongarch_asx) && defined(JSIMD_WITH_LASX)
  simd_support |= JSIMD_LASX;
#endif
#endif

  return simd_support;
}
