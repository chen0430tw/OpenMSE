'DVDCollectionDataset 的实际代码，定义用户所有的实用函数以处理此数据集
'和与 Amazon.com 返回的自定义 DVD 对象的交互
'使用分部类编译一个将设计器代码和用户代码组合在一起的数据集/数据表类型。
'应将该数据集/数据表的所有用户拥有的代码添加到此文件中。

Partial Public Class DVDCollectionDataSet

    Partial Public Class DVDsDataTable

        ''' <summary>
        ''' 重写默认的新行行为，以确保为每个记录创建唯一的 ID。  
        ''' </summary>
        ''' <remarks>需要唯一的 ID，以正确地查找行，并避免与类似的行冲突。ID 将作为 GUID。</remarks>
        Private Sub dvdsDataTable_TableNewRow(ByVal sender As Object, ByVal e As System.Data.DataTableNewRowEventArgs) Handles Me.TableNewRow
            Dim newDVDRow As DVDsRow = TryCast(e.Row, DVDsRow)
            If newDVDRow IsNot Nothing Then
                newDVDRow.ID = Guid.NewGuid.ToString
                newDVDRow.Title = "[新行 - 选择以输入详细信息]"
            End If
        End Sub

        ''' <summary>
        ''' 重载，通过传递一个包含在自定义 DVD 对象中的 DVD 行，将该 DVD 行添加到数据表。   
        ''' </summary>
        ''' <param name="myDVD">自定义 DVD 对象，应被转换和添加到数据表</param>
        Public Overloads Sub AddDvdsRow(ByVal myDVD As DVD)
            Me.AddDVDsRow(Me.DVDsRowFromDVDObject(myDVD))
        End Sub

        ''' <summary>
        ''' 实用工具函数，返回由自定义 DVD 对象转换的新 DVDRow 对象。  
        ''' </summary>
        ''' <param name="myDvd">将被转换为 DVDRow 的自定义 DVD 对象。</param>
        ''' <returns>由 DVD 对象参数初始化的新 DVDRow 对象。  </returns>
        ''' <remarks>在此特定数据表实例的上下文中创建新 DVDRow。</remarks>
        Public Function DVDsRowFromDVDObject(ByVal myDvd As DVD) As DVDCollectionDataSet.DVDsRow
            '用于存储新行值的对象 
            Dim dvdRow As DVDCollectionDataSet.DVDsRow

            '在以下数据表的上下文中创建新行: “Me”
            dvdRow = Me.NewDVDsRow()

            '尝试将 DVD 对象转换为 DVDRow
            Try
                With dvdRow
                    '执行简单值的复制
                    .Title = myDvd.Title
                    .Actors = myDvd.Actors
                    .Director = myDvd.Directors
                    .Rated = myDvd.Rating
                    .YearReleased = myDvd.ReleasedDate
                    .UPC = myDvd.UPC
                    .ImageLink = myDvd.ImageUrl
                    .WebPageLink = myDvd.WebPageUrl
                    .Description = myDvd.Description

                    '初始化用户设置的字段; 这些字段在输入时不进行转换
                    .MyRating = -1
                    .Comments = ""
                    .Genre = ""

                    '为避免问题执行更多转换:
                    '

                    '将长度值从字符串转换为整数
                    If (myDvd.RunningTime.Length > 0) Then
                        .Length = CInt(myDvd.RunningTime)
                    Else
                        .Length = 0
                    End If

                    '将图像转换为字节数组以保存在数据库中
                    If myDvd.ImageCache IsNot Nothing Then
                        Using memStream As New System.IO.MemoryStream()
                            myDvd.ImageCache.Save(memStream, myDvd.ImageCache.RawFormat)

                            .ImageBinary = memStream.GetBuffer()
                        End Using
                    End If

                End With
            Catch ex As Exception
                My.Application.Log.WriteEntry("试图将 DVD 对象转换为 DVDTableRow 时出错。")
                My.Application.Log.WriteException(ex)
            End Try

            Return dvdRow
        End Function

    End Class
End Class
