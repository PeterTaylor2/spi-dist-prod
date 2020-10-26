Attribute VB_Name = "clib_load_addins"

Public Const XLL = "xl_clib.xll"
Option Explicit

Public Sub Auto_Open()

    Dim myPath As String
    Dim curPath As String

'
' Change to the directory which contains this workbook
' Then we can safely open the XLL and find all of its DLLs
'
    myPath = spxlFile.Dirname(ThisWorkbook.FullName)
    curPath = CurDir()
    ChDrive myPath
    ChDir myPath

'
' Register the XLL
'
    If Not (Application.RegisterXLL(XLL)) Then
        MsgBox("Failed to load " + XLL)
    End If

    spxlMenu.Add "clib", "clib"
End Sub

Public Sub Auto_Close()
    spxlMenu.Remove "clib", "clib"
    Application.ExecuteExcel4Macro("RETURN(UNREGISTER(""" + XLL + """))")
End Sub
