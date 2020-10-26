Attribute VB_Name = "spxlFile"
Option Explicit
Option Base 0
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
