VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "PR sample program"
   ClientHeight    =   1860
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   6720
   LinkTopic       =   "Form1"
   ScaleHeight     =   1860
   ScaleWidth      =   6720
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton Command4 
      Caption         =   "Clear"
      Height          =   375
      Left            =   5280
      TabIndex        =   9
      Top             =   120
      Width           =   1335
   End
   Begin VB.CommandButton Command3 
      Caption         =   "Classify"
      Height          =   375
      Left            =   3720
      TabIndex        =   8
      Top             =   120
      Width           =   1455
   End
   Begin VB.CommandButton Command2 
      Caption         =   "Export"
      Height          =   375
      Left            =   1920
      TabIndex        =   7
      Top             =   120
      Width           =   1695
   End
   Begin VB.TextBox Text3 
      Height          =   285
      Left            =   1320
      TabIndex        =   3
      Text            =   "Text3"
      Top             =   1440
      Width           =   5295
   End
   Begin VB.TextBox Text2 
      Height          =   285
      Left            =   1320
      TabIndex        =   2
      Text            =   "Text2"
      Top             =   1080
      Width           =   5295
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Left            =   1320
      TabIndex        =   1
      Text            =   "Text1"
      Top             =   720
      Width           =   5295
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Import"
      Height          =   375
      Left            =   0
      TabIndex        =   0
      Top             =   120
      Width           =   1815
   End
   Begin VB.Label Label3 
      Caption         =   "resource file"
      Height          =   255
      Left            =   120
      TabIndex        =   6
      Top             =   1440
      Width           =   1095
   End
   Begin VB.Label Label2 
      Caption         =   "resources dir"
      Height          =   255
      Left            =   120
      TabIndex        =   5
      Top             =   1080
      Width           =   975
   End
   Begin VB.Label d 
      Caption         =   "dat file"
      Height          =   255
      Left            =   120
      TabIndex        =   4
      Top             =   720
      Width           =   615
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub Command1_Click()
 result = prImportDat(Text1.Text, Text2.Text, Text3.Text)
 MsgBox ("The given result was " & result)
End Sub

Private Sub Command2_Click()
 result = prExportDat(Text1.Text, Text2.Text, Text3.Text)
 MsgBox ("The given result was " & result)
End Sub

Private Sub Command3_Click()
 result = prVerifyDatType(Text1.Text)
 MsgBox ("The given result was " & result)
End Sub

Private Sub Command4_Click()
 result = prClearRes(Text3.Text)
 MsgBox ("The given result was " & result)
End Sub
