'03/09/2017 - Jason Rush - Modified by Jason Rush at University of Nevada, Reno
'07/21/2017 - Jason Rush - Bug fixes
'08/07/2019 - Jason Rush - Fixed column being added to the right of Totals, disabled screen updating during calcuations so its faster

'function copies entire row of data from "AllDonations" to the selected sheet
'takes a sheet as a string and two integer indices as arguments (start and stop indices)
'returns an integer
Function CopyData(SheetForSelection As String, TotalIndex As Integer, TableIndex As Integer, TerminateCell As Integer) As Integer
    Dim temp As Variant
    Sheets("AllDonations").Select
    temp = Range(Cells(TotalIndex, "A"), Cells(TotalIndex, TerminateCell)).Value    'load the values of the range from column A "1983" until the last available year into "temp"
    Sheets(SheetForSelection).Select                                                'select proper spreadsheet to work with
    Range(Cells(TableIndex, "A"), Cells(TableIndex, TerminateCell)).Value = temp    'transfer all values from AllDonations into proper worksheet index
    Sheets("AllDonations").Select                                                   'select AllDonations worksheet again
    CopyData = TableIndex + 1                                                       'update index for proper storage of data in next sync
End Function

'debugging function, ignore
Sub WriteArrayToImmediateWindow(arrSubA As Variant)

Dim rowString As String
Dim iSubA As Long
Dim jSubA As Long

rowString = ""

Debug.Print
Debug.Print
Debug.Print "The array is: "
For iSubA = 1 To UBound(arrSubA, 1)
    rowString = arrSubA(iSubA, 1)
    For jSubA = 2 To UBound(arrSubA, 2)
        rowString = rowString & "," & arrSubA(iSubA, jSubA)
    Next jSubA
    Debug.Print rowString
Next iSubA

End Sub

'Calculates the totals for a given sheet
'takes a sheet as a string and two integer indices (start and stop)
Sub CalculateTotals(SheetForSelection As String, TableIndex As Integer, TerminateCell As Integer)
    'Index represents the x-axis as we scan left to right
    'tableindex represents the y axis (up down)
    'terminatecell represents the furthest right cell before the Totals
    Dim Index As Integer
    Index = 0

    'select education worksheet
    Sheets(SheetForSelection).Select

    'the next two lines clear the two rows below the last Y entry in each sheet
    Range(Cells(TableIndex, "A"), Cells(TableIndex, TerminateCell)).Value = " "           'wipe row from A to column that represents current year + 1 just underneath last transfered set of values
    Range(Cells(TableIndex + 1, "A"), Cells(TableIndex + 1, TerminateCell)).Value = " "   'wipe row from A to column that represents current year + 1 just below one mentioned in previous comment
    Cells(TableIndex + 1, "A").Value = "Total"                                            'insert word "Total" into column A of row cleared in last line

    'calculate row totals on right side of each sheet
    For Index = 6 To TableIndex
        Set myRange = Range(Cells(Index, 3), Cells(Index, TerminateCell))
        Cells(Index, TerminateCell + 1) = "$" & Application.WorksheetFunction.Sum(myRange)
    Next

    'calculate column totals that runs across bottom of each sheet (plus one for Total in bottom right)
    'iterate rows from the starting to ending index (plus one for Total in bottom right)
    For Index = 3 To TerminateCell + 1
        'range is vertically oriented from (6,3) down to (TableIndex,3) to make sum for each column at the bottom of each sheet
        Set myRange = Range(Cells(6, Index), Cells(TableIndex, Index))

        'insert dollar sign into cell and calculate sum for myRange across bottom of each sheet
        Cells(TableIndex + 1, Index).Value = "$" & Application.WorksheetFunction.Sum(myRange)
    Next



    'calculate final total for sheet in bottom right corner
    'Set myRange = Range(Cells(6, TerminateCell + 1), Cells(TableIndex - 1, TerminateCell + 1))
    'Cells(TableIndex + 1, TerminateCell + 1) = Application.WorksheetFunction.Sum(myRange)
    'Cells(TableIndex + 1, TerminateCell) = ""
    'Cells(TableIndex, TerminateCell + 1) = ""

End Sub

'core logic loop that triggers when button on AllDonations is clicked
Sub UpdateButton()
    Application.ScreenUpdating = True
    Application.StatusBar = "Syncing pages..."
    Application.ScreenUpdating = False

    ' variables
    Dim mindex As Integer
    Dim myRange As Range
    Dim X As Variant
    Dim variable As String
    Dim temp As Variant
    Dim endCell As Variant
    Dim total As Integer
    Dim table1Index As Integer
    Dim table2Index As Integer
    Dim table3Index As Integer
    Dim table4Index As Integer
    Dim table5Index As Integer
    Dim table6Index As Integer
    Dim table7Index As Integer
    Dim table8Index As Integer
    Dim table9Index As Integer
    table1Index = 6
    table2Index = 6
    table3Index = 6
    table4Index = 6
    table5Index = 6
    table6Index = 6
    table7Index = 6
    table8Index = 6
    table9Index = 6
    currentYear = Year(Date)
    startingYear = 1983                          'the starting year of donations
    endCell = currentYear - startingYear + 3     'the end cell needs to be the cell just before the totals are generated
                                                     'this formula provides that end cell by factoring in the starting cell and the year
    Sheets("AllDonations").Select                'start on "AllDonations" spreadsheet

    For Index = 6 To 10000                       'iterate until the 10,000th row starting at row 6 (this is just future-proofing)
         variable = Cells(Index, "b").Value      'select the purpose code column
         If IsEmpty(variable) Then
             Index = 10000
         End If
         X = Split(variable, ",")                'acquire each "purpose code"
         For Each i In X                         'iterate through each "purpose code" to conduct specific logic
             If i = 1 Then                                                                           'trigger on purpose code '1'
                 table1Index = CopyData("Education", CInt(Index), CInt(table1Index), CInt(endCell))  'call function to copy data from main donation sheet to education sheet
             End If
             If i = 2 Then                                                                           'trigger on purpose code 2 (see above for details of logic done)
                 table2Index = CopyData("HealthCare", CInt(Index), CInt(table2Index), CInt(endCell))
             End If
             If i = 3 Then
                 table3Index = CopyData("HumRes", CInt(Index), CInt(table3Index), CInt(endCell))
             End If
             If i = 4 Then
                 table4Index = CopyData("ArtCult", CInt(Index), CInt(table4Index), CInt(endCell))
             End If
             If i = 5 Then
                 table5Index = CopyData("Religion", CInt(Index), CInt(table5Index), CInt(endCell))
             End If
             If i = 6 Then
                 table6Index = CopyData("EdSuppOutrch", CInt(Index), CInt(table6Index), CInt(endCell))
             End If
             If i = 7 Then
                 table7Index = CopyData("HPN", CInt(Index), CInt(table7Index), CInt(endCell))
             End If
             If i = 8 Then
                 table8Index = CopyData("AddTtmtRec", CInt(Index), CInt(table8Index), CInt(endCell))
             End If
             If i = 9 Then
                 table9Index = CopyData("WomInit", CInt(Index), CInt(table9Index), CInt(endCell))
             End If
         Next
     Next

     'TOTALS FOR SHEET ONE
         CalculateTotals "Education", CInt(table1Index), CInt(endCell)
     'TOTALS FOR SHEET TWO
         CalculateTotals "HealthCare", CInt(table2Index), CInt(endCell)
     'TOTALS FOR SHEET THREE
         CalculateTotals "HumRes", CInt(table3Index), CInt(endCell)
      'TOTALS FOR SHEET FOUR
         CalculateTotals "ArtCult", CInt(table4Index), CInt(endCell)
     'TOTALS FOR SHEET FIVE
         CalculateTotals "Religion", CInt(table5Index), CInt(endCell)
     'TOTALS FOR SHEET SIX
         CalculateTotals "EdSuppOutrch", CInt(table6Index), CInt(endCell)
     'TOTALS FOR SHEET SEVEN
         CalculateTotals "HPN", CInt(table7Index), CInt(endCell)
     'TOTALS FOR SHEET EIGHT
         CalculateTotals "AddTtmtRec", CInt(table8Index), CInt(endCell)
     'TOTALS FOR SHEET NINE
         CalculateTotals "WomInit", CInt(table9Index), CInt(endCell)
     'GO BACK TO MAIN PAGE AND DISPLAY MESSAGE

     Sheets("AllDonations").Select
     Application.ScreenUpdating = True
     Application.StatusBar = False
     MsgBox "Sync Complete"
End Sub
