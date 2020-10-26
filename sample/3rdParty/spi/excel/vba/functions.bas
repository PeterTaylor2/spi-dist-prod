Attribute VB_Name = "$(ns)Functions"
Option Explicit
Option Base 0

''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' This module contains functions which are specific to a particular
' product - as determined by namespace.
'
' All functions provided use the namespace as a prefix to the function
' name.
'
' We expect the following functions to be defined elsewhere:
'
' $(ns)AboutAddin
'     Should use a message box to provide information about version,
'     copyright, acknowledgements etc.
'
' $(ns)CloseAddin
'     This should unregister the XLL and remove the menu.
'     We provide functions $(ns)UnloadXLL and $(ns)RemoveMenu
'     to help with this operation. However in case there is more that
'     needs to be done this should be hand coded in the main module.
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

Private Const SERVICE_NAME As String = "$(serviceName)"
Private Const FUNCTION_PREFIX As String = "$(ns)"
Private Const XLL As String = "$(xll)"
Private Const START_LOGGING As String = "$(startLoggingFunction)"
Private Const STOP_LOGGING As String = "$(stopLoggingFunction)"

Public Sub $(ns)LoadXLL
    Dim myPath As String
    Dim curPath As String

'
' Add the directory which contains this workbook as the DLL directory
' Then we can safely open the XLL and find all of its DLLs
'
    myPath = $(ns)Utils.Dirname(ThisWorkbook.FullName)
    $(ns)Utils.AddDLLPath (myPath)
    
'
' Register the XLL
'
    Application.StatusBar = "Loading " & SERVICE_NAME & " addins..."
    If Not (Application.RegisterXLL(XLL)) Then
        MsgBox ("Failed to load " + XLL)
    End If
    
    $(ns)Utils.RemoveDLLPath
    Application.StatusBar = False

End Sub

Public Sub $(ns)UnloadXLL

    Application.ExecuteExcel4Macro ("RETURN(UNREGISTER(""" + XLL + """))")

End Sub

Public Sub $(ns)AddMenu

'
' This subroutine is called immediately after the XLL has been successfully
' loaded. At this point we expect to define the menu. We will be using the
' SERVICE_NAME as the main menu item.
'
    Const menuBarName As String = "Worksheet Menu Bar"
    Dim conbar As CommandBarControl
    Dim helpBar As CommandBar
    Dim helpIndex As Integer

    Dim controlCount As Integer
    Dim myBar As CommandBarControl
    Dim myItems As CommandBar
    Dim myBarIndex As Integer

    Dim button As CommandBarControl

    ' delete the old menu

    For Each conbar In CommandBars(menuBarName).Controls
        If conbar.Caption = SERVICE_NAME Then
            conbar.Delete
        End If
    Next

    ' determine the index of the standard "Help" Command bar control
    controlCount = 0
    For Each conbar In CommandBars(menuBarName).Controls
        If conbar.Caption = "&Help" Then
            helpIndex = conbar.Index
            GoTo endofLoop
        Else
            controlCount = controlCount + 1
        End If
    Next
    helpIndex = controlCount

endofLoop:

    Set helpBar = CommandBars(menuBarName).Controls(helpIndex).CommandBar

    ' now add my menu item

    Set myBar = CommandBars(menuBarName).Controls.Add(msoControlPopup, before:=helpIndex, temporary:=True)
    myBar.Visible = True
    myBar.Caption = SERVICE_NAME
    myBarIndex = myBar.Index

    Set myItems = CommandBars(menuBarName).Controls(myBarIndex).CommandBar

    Set button = myItems.Controls.Add(msoControlButton)
    button.Caption = "&About " & SERVICE_NAME
    button.OnAction = "$(ns)AboutAddin"
    button.BeginGroup = False ' don't add a separator bar before this control

    Set button = myItems.Controls.Add(msoControlButton)
    button.Caption = "&Close " & SERVICE_NAME
    button.OnAction = "$(ns)CloseAddin"
    button.BeginGroup = False ' don't add a separator before this control

    Set button = myItems.Controls.Add(msoControlButton)
    button.Caption = "&Start Logging..."
    button.OnAction = "$(ns)StartLogging"
    button.BeginGroup = True

    Set button = myItems.Controls.Add(msoControlButton)
    button.Caption = "S&top Logging"
    button.OnAction = "$(ns)StopLogging"
    button.BeginGroup = False
    
    Set button = myItems.Controls.Add(msoControlButton)
    button.Caption = "&Object Viewer"
    button.OnAction = "$(ns)LoadObjectViewer"
    button.BeginGroup = False
    
    Set button = myItems.Controls.Add(msoControlButton)
    button.Caption = "&User Guide"
    button.OnAction = "$(ns)LoadUserGuide"
    button.BeginGroup = False
    
    
End Sub

Public Sub $(ns)RemoveMenu
'
' This subroutine should be called immediately before the add-in is unloaded.
' It will remove the menu defined by $(ns)AddMenu.
'
    Const menuBarName As String = "Worksheet Menu Bar"
    Dim conbar As CommandBarControl
    Dim item As CommandBarButton

    ' delete our menu

    For Each conbar In CommandBars(menuBarName).Controls
        If conbar.Caption = SERVICE_NAME Then
            conbar.Delete
        End If
    Next
End Sub

Private Sub $(ns)StartLogging()
    ' starts logging allowing the user to choose the log filename
    Dim logfilename As String
    Dim result As String
    Dim filename As String

    filename = $(ns)Utils.OutputFilename("Log Files (*.log), *.log")
    If filename <> "False" Then
        result = Application.Run(START_LOGGING, filename)
    End If
End Sub

Private Sub $(ns)StopLogging()
    Dim result As Boolean
    result = Application.Run(STOP_LOGGING)
End Sub

Private Sub $(ns)LoadObjectViewer()

    $(ns)ObjectViewer.OBJECT_GET = "$(object_get)"
    $(ns)ObjectViewer.OBJECT_FREE = "$(object_free)"
    $(ns)ObjectViewer.OBJECT_COUNT = "$(object_count)"
    $(ns)ObjectViewer.OBJECT_TO_STRING = "$(object_to_string)"
    $(ns)ObjectViewer.Show
    
End Sub

Private Sub $(ns)LoadUserGuide()

    Dim myDirname As String
    Dim pdfFilename As String
        myDirname = dirname(ThisWorkbook.FullName)

    myDirname = $(ns)Utils.Dirname(ThisWorkbook.FullName)
    pdfFilename = $(ns)Utils.Filename(myDirName, "$(pdf).pdf")
    
    $(ns)Utils.ShellOper (pdfFilename)
    
End Sub


