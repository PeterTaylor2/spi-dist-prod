Attribute VB_Name = "spxlMenu"
Option Explicit
Option Base 0

Private theServiceName As String
Private theFunctionPrefix As String

Public Sub Add(serviceName As String, prefix As String)
'
' This subroutine is called immediately after the XLL has been successfully
' loaded. At this point we expect to define the menu. We will be using the
' serviceName as the main menu item.
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

    ' set the global variables
    theServiceName = serviceName
    theFunctionPrefix = prefix

    ' delete the old menu

    For Each conbar In CommandBars(menuBarName).Controls
        If conbar.Caption = serviceName Then
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
    myBar.Caption = serviceName
    myBarIndex = myBar.Index

    Set myItems = CommandBars(menuBarName).Controls(myBarIndex).CommandBar

    Set button = myItems.Controls.Add(msoControlButton)
    button.Caption = "&Close " & serviceName
    button.OnAction = "CloseAddin"
    button.BeginGroup = False ' don't add a separator before this control

    Set button = myItems.Controls.Add(msoControlButton)
    button.Caption = "&Start Logging..."
    button.OnAction = "StartLogging"
    button.BeginGroup = True

    Set button = myItems.Controls.Add(msoControlButton)
    button.Caption = "&Stop Logging"
    button.OnAction = "StopLogging"
    button.BeginGroup = False

End Sub


Public Sub Remove(serviceName As String, prefix As String)
'
' This subroutine is called immediately before the spxlMenu is unloaded. At this
' point we expect to remove the menu.
'
    Const menuBarName As String = "Worksheet Menu Bar"
    Dim conbar As CommandBarControl
    Dim item As CommandBarButton

    ' delete our menu

    For Each conbar In CommandBars(menuBarName).Controls
        If conbar.Caption = serviceName Then
            conbar.Delete
        End If
    Next
End Sub

Sub CloseAddin()
    Auto_Close
    ThisWorkbook.Close
End Sub

Sub StartLogging()
    ' starts logging allowing the user to choose the log filename
    Dim logfilename As String
    Dim result As String
    Dim filename As String

    filename = OutputFilename("Log Files (*.log), *.log")
    If filename <> "False" Then
        result = Application.Run(theFunctionPrefix & ".Logging.Start", filename)
    End If
End Sub

Sub StopLogging()
    Dim result As Boolean
    result = Application.Run(theFunctionPrefix & ".Logging.Stop")
End Sub
