/*=========================================================================

  Program:   ParaView
  Module:    vtkSMTrace.h

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkSMTrace
// .SECTION Description
//

#ifndef __vtkSMTrace_h
#define __vtkSMTrace_h

#include "vtkPVServerManagerCoreModule.h" // needed for exports
#include "vtkSmartPointer.h" // needed for iVar
#include "vtkSMObject.h"
#include "vtkStdString.h" // needed for ivar

class vtkSMProxy;
class SmartPyObject;

class VTKPVSERVERMANAGERCORE_EXPORT vtkSMTrace : public vtkSMObject
{
public:
  static vtkSMTrace* New();
  vtkTypeMacro(vtkSMTrace, vtkSMObject);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Provides access to the "active" tracer. There can only be one active tracer
  // in the application currently.
  static vtkSMTrace* GetActiveTracer()
    { return vtkSMTrace::ActiveTracer.GetPointer(); }

  // Description:
  // Methods to start/stop tracing. This will create a new instance of
  // vtkSMTrace and set that up as the active tracer. If an active tracer is
  // already present, then this will simply return the current active tracer.
  static vtkSMTrace* StartTrace();

  // Description:
  // Stop trace and return the generated trace script.
  // This will also destroy the active tracer.
  static vtkStdString StopTrace();

  // Description:
  // Get/Set whether all properties should be saved for a proxy,
  // including the default values. If false, only the properties
  // that have been modified from the XML-defaults will be logged.
  vtkSetMacro(TraceXMLDefaults, bool);
  vtkGetMacro(TraceXMLDefaults, bool);

  // Description:
  // Log generated trace to stdout as the trace is being generated
  // (useful for debugging).
  vtkSetMacro(LogTraceToStdout, bool);
  vtkGetMacro(LogTraceToStdout, bool);

  // Description:
  // Supplemental proxies are proxies that not explicitly created by the user
  // i.e. proxies such as lookup tables, scalar bars, animation scene, etc.
  // When set to true (default is false), the first time such a proxy is
  // encountered in the trace, the trace will log the property values on that
  // proxy using the PropertiesToTraceOnCreate rules.
  vtkSetMacro(FullyTraceSupplementalProxies, bool);
  vtkGetMacro(FullyTraceSupplementalProxies, bool);

  enum
    {
    RECORD_ALL_PROPERTIES=0,
    RECORD_MODIFIED_PROPERTIES=1,
    RECORD_USER_MODIFIED_PROPERTIES=2
    };

  vtkSetClampMacro(PropertiesToTraceOnCreate, int,
    RECORD_ALL_PROPERTIES, RECORD_USER_MODIFIED_PROPERTIES);
  vtkGetMacro(PropertiesToTraceOnCreate, int);

  // Description:
  // Return the current trace.
  vtkStdString GetCurrentTrace();

  // Description:
  // Save a Python state for the application and return it. Note this cannot be
  // called when tracing is active.
  static vtkStdString GetState(
    int propertiesToTraceOnCreate, bool skipHiddenRepresentations);

  // ************** BEGIN INTERNAL *************************
  // Description:
  // Internal class not meant to be used directly.
  class TraceItem;
  class VTKPVSERVERMANAGERCORE_EXPORT TraceItemArgs
    {
  public:
    TraceItemArgs();
    ~TraceItemArgs();

    // Overloads for keyword arguments.
    TraceItemArgs& arg(const char* key, vtkObject* val);
    TraceItemArgs& arg(const char* key, const char* val);
    TraceItemArgs& arg(const char* key, int val);
    TraceItemArgs& arg(const char* key, double val);
    TraceItemArgs& arg(const char* key, bool val);

    // Overloads for positional arguments.
    TraceItemArgs& arg(vtkObject* val);
    TraceItemArgs& arg(const char* val);
    TraceItemArgs& arg(int val);
    TraceItemArgs& arg(double val);
    TraceItemArgs& arg(bool val);

  private:
    TraceItemArgs(const TraceItemArgs&);
    void operator=(const TraceItemArgs&);

    friend class TraceItem;
    class vtkInternals;
    vtkInternals* Internals;
    };

  class VTKPVSERVERMANAGERCORE_EXPORT TraceItem
    {
  public:
    TraceItem(const char* type);
    ~TraceItem();
    void operator=(const TraceItemArgs& arguments);
  private:
    TraceItem(const TraceItem&);
    void operator=(const TraceItem&);
    const char* Type;
    class TraceItemInternals;
    TraceItemInternals* Internals;
    };
  // ************** END INTERNAL *************************

protected:
  vtkSMTrace();
  virtual ~vtkSMTrace();

  // Description:
  // Returns true of there's an error. Otherwise, returns false.
  bool CheckForError();

  bool TraceXMLDefaults;
  bool LogTraceToStdout;
  int PropertiesToTraceOnCreate;
  bool FullyTraceSupplementalProxies;

private:
  vtkSMTrace(const vtkSMTrace&); // Not implemented.
  void operator=(const vtkSMTrace&); // Not implemented.

  static vtkSmartPointer<vtkSMTrace> ActiveTracer;
  class vtkInternals;
  vtkInternals* Internals;

  friend class TraceItem;
  const SmartPyObject& GetTraceModule() const;
  const SmartPyObject& GetCreateItemFunction() const;
};

#define SM_SCOPED_TRACE_0(x, y) x ## y
#define SM_SCOPED_TRACE_1(x, y) SM_SCOPED_TRACE_0(x, y)
#define SM_SCOPED_TRACE(__A_TRACE_TYPE) \
  vtkSMTrace::TraceItem SM_SCOPED_TRACE_1(_trace_item,__LINE__)(#__A_TRACE_TYPE); \
  SM_SCOPED_TRACE_1(_trace_item,__LINE__) = vtkSMTrace::TraceItemArgs()
#endif
