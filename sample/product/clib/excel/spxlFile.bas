Attribute VB_Name = "spxlFile"
Option Explicit
Option Base 0
Private Declare Function SetDllDirectory Lib "kernel32.dll" Alias "SetDllDirectoryA" ( _
    ByVal lpPathName As String) As Boolean
  
Public Function AddDLLPath(ByRef path As String) As Boolean
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' This function sets the given path as the DLL directory
'
' This enables you to find all DLLs in the given directory
'
' Better than temporarily changing to the given directory since in some cases
' other threads might change it back to something else.
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
    Dim s As String
    s = path & Chr(0)
    AddDLLPath = SetDllDirectory(s)
    
End Function
Public Function RemoveDLLPath() As Boolean
    
    RemoveDLLPath = AddDLLPath("")

End Function

Function OutputFilename(filter As String) As String
    Dim myDirname As String

    myDirname = Dirname(ThisWorkbook.FullName)
    OutputFilename = Application.GetSaveAsFilename(myDirname, filter)
End Function
Function InputFileName(filter As String) As String
    Dim myDirname As String

    myDirname = Dirname(ThisWorkbook.FullName)
    ChDir myDirname
    ChDrive myDirname
    InputFileName = Application.GetOpenFilename(filter)
End Function
Function Dirname(myFilename As String) As String
' returns the directory name from the full path - equivalent to dirname in shell
    Dim i As Integer

    i = 0
    While InStr(i + 1, myFilename, Application.PathSeparator) > 0
        i = InStr(i + 1, myFilename, Application.PathSeparator)
    Wend
    If i = 0 Then
        Dirname = CurDir
    Else
        Dirname = Left(myFilename, i - 1)
    End If
End Function
Function Basename(myFilename As String) As String
' returns the basename from the full path - equivalent to basename in shell
    Dim i As Integer

    i = 0
    While InStr(i + 1, myFilename, Application.PathSeparator) > 0
        i = InStr(i + 1, myFilename, Application.PathSeparator)
    Wend
    Basename = Right(myFilename, Len(myFilename) - i)
End Function
Function Filename(myDirname As String, myBasename As String) As String
' combines directory name and file name to create a full file name
    Filename = myDirname + Application.PathSeparator + myBasename
End Function
