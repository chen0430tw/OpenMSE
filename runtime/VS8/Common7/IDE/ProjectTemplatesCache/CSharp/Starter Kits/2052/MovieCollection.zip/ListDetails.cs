using System;
using System.Data;
using System.Drawing;
using System.Diagnostics;
using System.IO;
using System.Windows.Forms;
using $safeprojectname$.Controls;

namespace $safeprojectname$
{
    /// <summary>
    /// 用户控件负责查看、管理和实时编辑收藏的电影的详细信息。  
    /// </summary>
    /// <remarks>
    /// 通过 SearchOnline 控件以编程的方式添加电影，或在按下“添加”按钮后手动添加。
    /// 可通过选择网格中的一个值实时编辑电影。可通过选择并按下“删除”按钮删除电影。
    /// 按下“联机观看”按钮以访问 Amazon.com 网站。按下“刷新”按钮 
    /// 用来自 Amazon.com Web 服务的实时信息刷新数据字段。
    /// 电影从 MovieDatabase.mdf 中读取并向其写入。建议定期刷新 
    /// 电影信息。  
    /// </remarks>
    public partial class ListDetails : UserControl
    {

        System.Windows.Forms.Form hostingForm;

        //负责对电影标题进行分级的控件对象。  
        //解决方法: 在使用“新建项目”向导后，项目无法动态生成此自定义控件类型；
        //                      控件实例将在运行时创建和布置，而不会
        //                      添加到设计图面。  
        internal RatingPickerControl ratingPickerControl;

		//在对数据库进行更新时打开。
		bool isUpdating;

        /// <summary>
        /// 初始化此控件并加载数据。    
        /// </summary>
        /// <remarks>
        /// 将收藏中的所有电影标题加载到内存中的数据表；  
        /// 此控件监视对数据表的更改，并自动保存对每个字段进行的更改。 
        /// </remarks>
        private void ListDetails_Load(System.Object sender, System.EventArgs e)
        {
            //动态创建分级选择器控件并布置在窗体上
            InitRatingPickerControl();

            //在用户界面中进行添加、修改和删除时侦听数据更改
            this.dvdCollectionDataSet.DVDs.TableNewRow += new DataTableNewRowEventHandler(this.dvdCollectionDataSet.DVDs.dvdsDataTable_TableNewRow);
            this.dvdCollectionDataSet.DVDs.RowChanged += new DataRowChangeEventHandler(dt_RowChanged);

            //从数据库加载数据
            if (this.LoadData() == 0){
                //如果无可用行，则创建新行，以便用户可以开始输入新的 DVD
                this.dvdsDataConnector.AddNew();
            }

            this.titleTextBox.Select();
        }

        /// <summary>
        /// 检测对数据表进行的任何更改并将更改保存到数据库。
        /// </summary>
        private void dt_RowChanged(object sender, System.Data.DataRowChangeEventArgs e)
        {
			if (isUpdating){
                return;
            }

            $safeprojectname$.DVDCollectionDataSet.DVDsRow dvdDataRow = ($safeprojectname$.DVDCollectionDataSet.DVDsRow) e.Row;
            if (dvdDataRow != null){
				SaveData();
			}
        }

        /// <summary>
        /// 单击“添加”按钮时在收藏中创建新电影。  
        /// </summary>
        /// <remarks>从 DataConnector 调用 AddNew，以保持控件与数据表之间的数据绑定同步这一做法非常可取。  </remarks>
        private void AddTitleButton_Click(System.Object sender, System.EventArgs e)
        {
            this.dvdsDataConnector.AddNew();
        }

        /// <summary>
        /// 删除当前选中的电影。
        /// </summary>
        private void RemoveButton_Click(System.Object sender, System.EventArgs e)
        {
            DeleteCurrent();
        }

        /// <summary>
        /// 单击时应用用户指定的筛选器。
        /// </summary>
        private void FilterButton_Clicked(System.Object sender, System.EventArgs e)
        {
            ApplyTextboxFilter();
        }

        /// <summary>
        /// 输入筛选器字符串时应用用户指定的筛选器。
        /// </summary>
        private void FilterTextBox_KeyUp(object sender, System.Windows.Forms.KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                ApplyTextboxFilter();
            }
        }

        /// <summary>
        /// 单击此按钮时将 DataConnector 重置为未筛选状态。
        /// </summary>
        private void ShowAllButton_Click(System.Object sender, System.EventArgs e)
        {
            this.dvdsDataConnector.Filter = "";
        }

        /// <summary>
        /// 在提示用户后用来自 Amazon.com Web 服务的数据刷新当前的电影选择。  
        /// </summary>
        /// <remarks>
        /// 使用 UPC 值从 Web 服务获取电影标题，并合并/重写 
        /// 现有的数据。“Comments”和“MyRating”字段是作为特例保留的。  
        /// </remarks>
        private void RefreshOnlineButton_Click(System.Object sender, System.EventArgs e)
        {
            //提示用户刷新并存储结果
            DialogResult dialogResult = MessageBox.Show("此功能可能会改写您的更改。要继续吗?", "是否从联机 Web 服务刷新数据?", MessageBoxButtons.YesNo);

            //如果是，则开始刷新过程
            if (dialogResult == DialogResult.Yes)
            {
                //用于调用 Amazon.com Web 服务的对象
                SimpleAmazonWS amazonService = new SimpleAmazonWS();

                //用于存储搜索结果的对象
                DVD refreshedDVD;
                $safeprojectname$.DVDCollectionDataSet.DVDsRow refreshedDVDRow;

                try
                {
                    //边界检查: 确保电影数据表不为空
                    if (this.dvdsDataConnector.Count > 0)
                    {
                        //获取当前电影选择并转换为类型化的电影行对象
                        DataRowView rowView = (DataRowView)this.dvdsDataConnector.Current;
                        $safeprojectname$.DVDCollectionDataSet.DVDsRow dvdRow = ($safeprojectname$.DVDCollectionDataSet.DVDsRow)rowView.Row;

                        // 检查 UPC 值是否为空
                        if ((!dvdRow.IsUPCNull()) && dvdRow.UPC != "")
                        {
                            //用于存储和保留自定义评价与分级值的变量
                            string currentComments = dvdRow.Comments;
                            int currentRating = dvdRow.MyRating;

                            //从 Web 服务中获取更新的 DVD 对象
                            refreshedDVD = amazonService.RefreshDVDByUPC(dvdRow.UPC);

                            //更新缓存的图像
                            this.boxArtPictureBox.Load(refreshedDVD.ImageUrl);
                            refreshedDVD.ImageCache = (Bitmap)this.boxArtPictureBox.Image;

                            //将 DVD 转换为 dvdRow
                            refreshedDVDRow = this.dvdCollectionDataSet.DVDs.DVDsRowFromDVDObject(refreshedDVD);

                            //设置为原始行的 ID，从而可以成功执行对内存中的数据表的行搜索
                            refreshedDVDRow.ID = dvdRow.ID;

                            //恢复/合并回自定义评价和分级值
                            refreshedDVDRow.Comments = currentComments;
                            refreshedDVDRow.MyRating = currentRating;

                            //将 dvdRow 与匹配行合并
                            this.dvdCollectionDataSet.DVDs.LoadDataRow(refreshedDVDRow.ItemArray, LoadOption.Upsert);
                        }
                    }

                }
                catch (Exception ex)
                {
                    MessageBox.Show(String.Format("从联机 Web 服务刷新数据时出现问题: {0}", ex.Message));
                    Debug.WriteLine(ex.ToString());
                }
            }

        }

        /// <summary>
        /// 提示用户后，在外部浏览器中导航到 Amazon.com 网站上与当前电影选择有关的网页。  
        /// </summary>
        /// <remarks>在启动浏览器进程和导航到 URL 之前，用户应该可以看到此超链接。  </remarks>
        private void ViewOnlineButton_Click(System.Object sender, System.EventArgs e)
        {
            //边界检查: 确保电影数据表不为空
            if (this.dvdsDataConnector.Count > 0)
            {
                //用于存储和保留自定义评价与分级值的变量
                DataRowView rowView = (DataRowView)this.dvdsDataConnector.Current;
                DVDCollectionDataSet.DVDsRow dvdRow = ($safeprojectname$.DVDCollectionDataSet.DVDsRow)rowView.Row;

                //用于存储网页 URL 值的变量
                string webLink = "";

                //访问 WebPageLink 属性之前检查其是否为空
                if (!dvdRow.IsWebPageLinkNull()) { webLink = dvdRow.WebPageLink; }

                //提示查看最终用户是否信任 URL 并存储提示结果值
                string msgText = String.Format("将要在外部浏览器中查看此 URL:“{0}”。继续操作是否安全?", webLink);
                DialogResult yesNoResult = MessageBox.Show(msgText, "确定要浏览?", MessageBoxButtons.YesNo);

                //如果用户接受提示信息，则启动 URL 路径
                if ((yesNoResult == DialogResult.Yes) && (webLink != ""))
                {
                    Process.Start(webLink);
                }
            }
        }

        /// <summary>
        /// 单击时提示用户导入文件。  
        /// </summary>
        private void ImportButton_Click(object sender, System.EventArgs e)
        {
            this.openFileDialog1.ShowDialog();
        }

        /// <summary>
        /// 用户单击“确定”打开文件提示时，导入用户提供的图像。   
        /// </summary>
        private void OpenFileDialog1_FileOk(System.Object sender, System.ComponentModel.CancelEventArgs e)
        {
            this.ImportImage(this.openFileDialog1.FileName);
        }

        /// <summary>
        /// 加载电影收藏数据。  
        /// </summary>
        /// <returns>正整数表示加载的行数；若没有加载行则为 0。  </returns>
        /// <remarks>将电影收藏数据加载到内存中的数据表</remarks>
        internal int LoadData()
        {
            int rowsLoaded = 0;

            //试图使用对所有电影的查询来填充数据表
            //注意: 如果数据库中有大量的电影，您可能要添加对
            //TableAdapter 的查询，以便只将数据库中的一部分电影填入内存中。
            try
            {
                isUpdating = true;
                rowsLoaded = this.dvdsTableAdapter.Fill(this.dvdCollectionDataSet.DVDs);
            }
            catch (Exception ex)
            {
                MessageBox.Show(String.Format("加载数据时出现问题: {0}", ex.Message));
                Debug.WriteLine(ex.ToString());
            }
            finally
            {
                isUpdating = false;
            }

            return rowsLoaded;
        }

        /// <summary>
        /// 保存对电影收藏数据进行的更改。  
        /// </summary>
        /// <returns>正整数表示数据库中被更新的行数；若没有行被更新则为 0。  </returns>
        /// <remarks>保存来自内存中的数据表和绑定 UI 的更改。  </remarks>
        internal int SaveData()
        {
            if (isUpdating){
                return 0;
            }

            int rowsSaved = 0;

            //尝试获取更改并将更改保存到数据库
            try
            {
                //只在检测到更改时更新数据库
                if (dvdCollectionDataSet.HasChanges())
                {
					isUpdating = true;
                    rowsSaved = this.dvdsTableAdapter.Update(this.dvdCollectionDataSet.DVDs);
                }
            }
            catch 
            {
            }
			finally
			{
				isUpdating = false;
			}
            return rowsSaved;
        }

        /// <summary>
        /// 使用文本框中用户指定的输入对内存中的电影记录应用筛选器。  
        /// </summary>
        /// <remarks>DataConnector 对象的“筛选器”属性适用于绑定到 DataConnector 的所有控件。 </remarks>
        private void ApplyTextboxFilter()
        {
            try
            {
                //将筛选字符串 WHERE 子句设置为“Where COLUMNNAME like '@FilterText'”的格式
                this.dvdsDataConnector.Filter = String.Format("{0} like '%{1}%'", this.dvdCollectionDataSet.DVDs.TitleColumn.ColumnName, this.filterTextBox.Text);
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.ToString());
            }

        }

        /// <summary>
        /// 提示用户后删除当前选中的电影。
        /// </summary>
        internal void DeleteCurrent()
        {
            //执行删除之前执行边界检查: 数据表是否为空，或者是否未进行选择
            if ((this.dvdsDataConnector.Count > 0) && (this.dvdsDataConnector.Current != null))
            //将 DataConnector 返回的泛型 Current 对象转换为类型化的电影行对象
            {
                DataRowView rowView = (DataRowView)this.dvdsDataConnector.Current;
                $safeprojectname$.DVDCollectionDataSet.DVDsRow dvdRow = ($safeprojectname$.DVDCollectionDataSet.DVDsRow)rowView.Row;

                //表示要删除的当前电影的友好名称的字符串
                string title = "[无标题]";

                //使用类型化行对象中的电影标题属性(如果该属性包含有效数据)
                if ((!dvdRow.IsTitleNull()) && (dvdRow.Title != null))
                {
                    title = dvdRow.Title;
                }

                //删除前在用户提示中显示的文字
                string msgText = String.Format("确实要从电影收藏中永久删除 {0}? ", title);

                //向用户显示“是/否”提示并存储结果
                DialogResult dialogResult = MessageBox.Show(msgText, "确定要删除?", MessageBoxButtons.YesNo);

                //如果用户接受提示信息，则移除当前电影
                if (dialogResult == DialogResult.Yes)
                {
                    this.dvdsDataConnector.RemoveCurrent();
                }

                //确保始终有可用行
                if (dvdsDataConnector.Count == 0){
                    this.dvdsDataConnector.AddNew();
                }
            }
        }

        /// <summary>
        /// 初始化此用户控件使用的分级选择器控件。  
        /// </summary>
        /// <remarks>
        /// 由 ListDetails.Load 事件调用。若在单独的控件库中生成和引用 RatingPickerControl， 
        /// 则无需此特殊的初始化代码。  
        /// </remarks>
        private void InitRatingPickerControl()
        {
            //创建分级选择器控件对象并将其布置在父用户控件上
            this.ratingPickerControl = new RatingPickerControl();
            this.dvdDetailsPanel.Controls.Add(this.ratingPickerControl);

            //设置默认属性和数据绑定
            ((System.ComponentModel.ISupportInitialize)this.ratingPickerControl).BeginInit();
            this.ratingPickerControl.Anchor = (System.Windows.Forms.AnchorStyles)(System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right);
            this.ratingPickerControl.BackColor = System.Drawing.Color.Transparent;
            this.ratingPickerControl.CurrentRating = -1;
            this.ratingPickerControl.DataBindings.Add("CurrentRating", this.dvdsDataConnector, "MyRating", true, DataSourceUpdateMode.OnPropertyChanged, null);
            this.ratingPickerControl.Location = new System.Drawing.Point(128, 388);
            this.ratingPickerControl.Name = "RatingPickerControl1";
            this.ratingPickerControl.Size = new System.Drawing.Size(80, 16);
            this.ratingPickerControl.TabIndex = 7;
            ((System.ComponentModel.ISupportInitialize)this.ratingPickerControl).EndInit();
        }

        /// <summary>
        /// 从用户提供的文件中将图像值导入到数据表。  
        /// </summary>
        /// <remarks></remarks>
        private void ImportImage(string filename)
        {
            try
            {
                //执行删除之前执行边界检查: 数据表是否为空，或者是否未进行选择
                if (this.dvdsDataConnector.Current != null)
                {
                    //将 DataConnector 返回的泛型 Current 对象转换为类型化的电影行对象
                    DataRowView rowView = (DataRowView)this.dvdsDataConnector.Current;
                    $safeprojectname$.DVDCollectionDataSet.DVDsRow dvdRow = ($safeprojectname$.DVDCollectionDataSet.DVDsRow)rowView.Row;

                    //从文件系统以只读方式打开文件，复制字节，然后赋给当前行的图像属性
                    dvdRow.ImageBinary = File.ReadAllBytes(filename);

                    this.dvdsDataConnector.ResetCurrentItem();
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(String.Format("加载图像时出现问题: {0}", ex.Message));
                Debug.WriteLine(ex.ToString());
            }
        }

        private void HostingForm_FormClosed(System.Object sender, System.Windows.Forms.FormClosedEventArgs e){
            //调用 EndEdit 来捕获 UI 中所做的所有更改
            this.dvdsDataConnector.EndEdit();

            //当父窗体关闭时，请确保对此控件的数据进行保存
            SaveData();
        }

        private void ListDetails_ParentChanged(System.Object sender, System.EventArgs e){
            ////如果此用户控件的父窗体进行过更改，则需要重置新父窗体的 Form_Closed 事件处理程序
            if (hostingForm != null){
                this.hostingForm.FormClosed -= new System.Windows.Forms.FormClosedEventHandler(HostingForm_FormClosed);
            }
            this.hostingForm = (System.Windows.Forms.Form)this.ParentForm;
            this.hostingForm.FormClosed += new System.Windows.Forms.FormClosedEventHandler(HostingForm_FormClosed);
        }

        private void TitleTextBox_Click(System.Object sender, System.EventArgs e){
            if (this.titleTextBox.Text == "[新行 - 选择以输入详细信息]"){
                this.titleTextBox.SelectAll();
            }
        }
    }
}


