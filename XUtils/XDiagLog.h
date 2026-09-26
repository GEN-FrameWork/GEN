/**-------------------------------------------------------------------------------------------------------------------
*
* @file       XDiagLog.h
*
* @brief      TEMPORARY diagnostic-only logging helper, added 2026-09 to root-cause a multi-second full-dashboard
*             blank-freeze reported on real hardware and VMs (Windows and Linux) that could not be reproduced or
*             explained from a sandboxed reconstruction alone. NOT part of the GEN framework proper -- delete this
*             file and every '#include "XDiagLog.h"' / XDIAGLOG_* call site once the freeze is root-caused and
*             fixed.
*
*             Header-only on purpose: every call site (UI_System.cpp, UI_Layout.cpp, UI_SkinCanvas.cpp, ...) needs
*             ONE shared, correlated wall-clock reference and log file, but adding a new .cpp to every platform's
*             CMake source list is exactly the kind of footprint a temporary tool should not require. An `inline`
*             function's local statics are merged by the linker into a single instance across every translation
*             unit that includes this header (ODR), so XDIAGLOG_NOWUS() below returns the SAME elapsed-since-
*             first-call clock everywhere, letting log lines from different .cpp files be laid on one timeline
*             with no shared header state beyond this file.
*
*             Uses GEN_XFACTORY.CreateTimer() (a real platform wall-clock XTIMER, microsecond resolution) -- NOT
*             clock()/CPU time, which would hide a genuine block/wait (blocked-on-I/O or blocked-on-mutex time is
*             NOT CPU time and would read as ~0us, exactly the false negative that must be avoided here.
*
* @copyright  EndoraSoft. All rights reserved.
*
* @class      XDIAGLOG
* @ingroup    XUTILS
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
#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdarg.h>

#include "XBase.h"
#include "XTimer.h"
#include "XFactory.h"


/*---- INLINE FUNCTIONS -----------------------------------------------------------------------------------------------*/

// Shared, process-wide elapsed-microseconds clock. First call anywhere establishes t=0; every later call, from any
// TU, reports elapsed time against that SAME origin -- this is what makes log lines from different files
// comparable on one timeline.
inline XQWORD XDiagLog_ElapsedMicroSeconds()
{
  static XTIMER* xdiaglog_timer = GEN_XFACTORY.CreateTimer();
  static bool    xdiaglog_ini   = false;

  if(xdiaglog_timer && !xdiaglog_ini)
    {
      xdiaglog_timer->Reset();
      xdiaglog_ini = true;
    }

  return xdiaglog_timer ? xdiaglog_timer->GetMeasureMicroSeconds() : 0;
}


// Appends one line to "uidiag.log" (relative to the running executable's working directory) as:
//   [tag] t=<seconds>.<microseconds> <formatted message>
// Opens/closes the file per call (no held handle, no extra mutex) -- fine for a low-frequency diagnostic tool,
// not meant to survive as production logging.
inline void XDiagLog_Write(const char* tag, const char* fmt, ...)
{
  FILE* xdiaglog_file = fopen("uidiag.log", "a");
  if(!xdiaglog_file) return;

  XQWORD nowus = XDiagLog_ElapsedMicroSeconds();

  fprintf(xdiaglog_file, "[%s] t=%llu.%06llu ", tag, (unsigned long long)(nowus/1000000ULL), (unsigned long long)(nowus%1000000ULL));

  va_list args;
  va_start(args, fmt);
  vfprintf(xdiaglog_file, fmt, args);
  va_end(args);

  fprintf(xdiaglog_file, "\n");

  fclose(xdiaglog_file);
}


/*---- CONVENIENCE MACROS ---------------------------------------------------------------------------------------------*/

#define XDIAGLOG_NOWUS()          XDiagLog_ElapsedMicroSeconds()
#define XDIAGLOG_WRITE(tag, ...)  XDiagLog_Write(tag, __VA_ARGS__)
