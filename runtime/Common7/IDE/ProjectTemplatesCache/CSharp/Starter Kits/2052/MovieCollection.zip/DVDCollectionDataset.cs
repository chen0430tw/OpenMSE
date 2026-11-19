using System;
using System.Diagnostics;
using System.IO;
using $safeprojectname$.Controls;

namespace $safeprojectname$
{
    //DVDCollectionDataset 的实际代码，定义用户所有的处理此数据集的实用函数，
    //并定义与 Amazon.com 返回的自定义 DVD 对象的交互。
    //使用分部类编译一个将设计器代码和用户代码组合在一起的数据集/数据表类型。
    //应将该数据集/数据表的所有用户拥有的代码添加到此文件中。
    public partial class DVDCollectionDataSet
    {
        public partial class DVDsDataTable
        {

            /// <summary>
            /// 重写新行的默认行为，以确保为每条记录创建唯一的 ID。  
            /// </summary>
            /// <remarks>需要唯一的 ID，以正确地查找行，并避免与类似的行冲突。ID 将作为 GUID。  </remarks>
            internal void dvdsDataTable_TableNewRow(object sender, System.Data.DataTableNewRowEventArgs e)
            {
                DVDsRow newDVDRow = e.Row as DVDsRow;
                if (newDVDRow != null)
                {
                    newDVDRow.ID = Guid.NewGuid().ToString();
                    newDVDRow.Title = "[新行 - 选择以输入详细信息]";
                }
            }

            /// <summary>
            /// 重载，通过传递一个包含在自定义 DVD 对象中的 DVD 行，将该 DVD 行添加到数据表。  
            /// </summary>
            /// <param name="myDVD">自定义 DVD 对象，应将其转换并添加到数据表</param>
            public void AddDVDsRow(DVD myDVD)
            {
                AddDVDsRow(this.DVDsRowFromDVDObject(myDVD));
            }

            /// <summary>
            /// 实用工具函数，返回一个从自定义 DVD 对象转换而来的新 DVDRow 对象。
            /// </summary>
            /// <param name="myDvd">自定义 DVD 对象，将被转换为 DVDRow。  </param>
            /// <returns>由 DVD 对象参数初始化的新 DVDRow 对象。  </returns>
            /// <remarks>在此特定数据表实例的上下文中创建新 DVDRow。  </remarks>
            public DVDsRow DVDsRowFromDVDObject(DVD myDvd)
            {
                //用于存储新行值的对象 
                DVDsRow dvdRow;

                //在以下数据表的上下文中创建新行: “Me”
                dvdRow = NewDVDsRow();

                //尝试将 DVD 对象转换为 DVDRow
                try
                {
                    dvdRow.Title = myDvd.Title;
                    dvdRow.Actors = myDvd.Actors;
                    dvdRow.Director = myDvd.Directors;
                    dvdRow.Rated = myDvd.Rating;
                    dvdRow.YearReleased = myDvd.ReleasedDate;
                    dvdRow.UPC = myDvd.UPC;
                    dvdRow.ImageLink = myDvd.ImageUrl;
                    dvdRow.WebPageLink = myDvd.WebPageUrl;
                    dvdRow.Description = myDvd.Description;

                    //初始化用户设置的字段; 这些字段在输入时不进行转换
                    dvdRow.MyRating = -1;
                    dvdRow.Comments = "";
                    dvdRow.Genre = "";

                    //为避免问题执行更多转换:
                    //

                    //将长度值从字符串转换为整数
                    if (myDvd.RunningTime.Length > 0)
                    {
                        dvdRow.Length = Int32.Parse(myDvd.RunningTime);
                    }
                    else
                    {
                        dvdRow.Length = 0;
                    }

                    //将图像转换为字节数组以保存在数据库中
                    if (myDvd.ImageCache != null)
                    {
                        using (MemoryStream memStream = new MemoryStream())
                        {
                            myDvd.ImageCache.Save(memStream, myDvd.ImageCache.RawFormat);
                            dvdRow.ImageBinary = memStream.GetBuffer();
                        }
                    }

                }
                catch (Exception ex)
                {
                    Debug.WriteLine("试图将 DVD 对象转换为 DVDTableRow 时出错。");
                    Debug.WriteLine(ex);
                }

                return dvdRow;
            }
        }
    }
}