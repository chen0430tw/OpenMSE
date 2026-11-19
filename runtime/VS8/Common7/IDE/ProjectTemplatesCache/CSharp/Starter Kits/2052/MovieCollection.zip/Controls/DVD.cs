using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;
using System.Diagnostics;

namespace $safeprojectname$.Controls
{
    /// <summary>
    /// 表示 DVD 标题。
    /// </summary>
    /// <remarks>SimpleAmazon Web 服务类返回这些 DVD 类的泛型列表。</remarks>
    public class DVD
    {

        private string m_UPC = "";
        private string m_Title = "";
        private string m_Description = "";
        private string[] m_Actors = new string[]{};
        private string[] m_Directors = new string[]{};
        private string m_Rating = "";
        private string m_RunningTime = "";
        private string m_ReleasedDate = "";
        private string m_WebPageUrl = "";
        private string m_ImageUrl = "";
        public Bitmap ImageCache = null;

        public DVD()
        {

        }

        public DVD(string title, string upc, string description)
        {
            m_UPC = CheckString(upc);
            m_Title = CheckString(title);
            m_Description = CheckString(description);
        }

        public DVD(string title, string upc, string description, string[] actors, string[] directors, string rating, string runningtime, string releasedDate, string webPageUrl, string imageUrl)
        {
            m_UPC = CheckString(upc);
            m_Title = CheckString(title);
            m_Description = CheckString(description);
            m_Actors = actors;
            m_Directors = directors;
            m_Rating = CheckString(rating);
            m_RunningTime = CheckString(runningtime);
            m_ReleasedDate = CheckString(releasedDate);
            m_WebPageUrl = CheckString(webPageUrl);
            m_ImageUrl = CheckString(imageUrl);
        }

        public DVD(string title, string upc, string description, string actors, string directors, string rating, string runningtime, string releasedDate, string webPageUrl, string imageUrl)
        {
            m_UPC = CheckString(upc);
            m_Title = CheckString(title);
            m_Description = CheckString(description);
            m_Actors[0] = actors;
            m_Directors[0] = directors;
            m_Rating = CheckString(rating);
            m_RunningTime = CheckString(runningtime);
            m_ReleasedDate = CheckString(releasedDate);
            m_WebPageUrl = CheckString(webPageUrl);
            m_ImageUrl = CheckString(imageUrl);
        }

        public string UPC
        {
            get
            {
                return this.m_UPC;
            }
        }

        public string Title
        {
            get
            {
                return m_Title;
            }
        }

        public string Description
        {
            get
            {
                return m_Description;
            }
        }

        public string Actors
        {
            get
            {
                string singleString = "";
                try
                {
                    if (this.m_Actors != null)
                    {
                        foreach (string part in this.m_Actors)
                        {
                            if (singleString == "")
                            {
                                singleString = part;
                            }
                            else
                            {
                                if (part != null) { singleString = singleString + ";" + part; }
                            }
                        }
                    }
                }
                catch (Exception ex)
                {
                    Debug.WriteLine(ex);
                }

                return singleString;
            }
        }

        public string Directors
        {
            get
            {
                string singleString = "";
                try
                {
                    if (this.m_Directors != null)
                    {
                        foreach (string part in this.m_Directors)
                        {
                            if (singleString == "")
                            {
                                singleString = part;
                            }
                            else
                            {
                                if (part != null) { singleString = singleString + ";" + part; }
                            }
                        }
                    }

                }
                catch (Exception ex)
                {
                    Debug.WriteLine(ex);
                }

                return singleString;
            }
        }

        public string ImageUrl
        {
            get
            {
                return this.m_ImageUrl;
            }
            set
            {
                m_ImageUrl = value;
            }
        }

        public string Rating
        {
            get
            {
                return this.m_Rating;
            }
        }

        public string ReleasedDate
        {
            get
            {
                return this.m_ReleasedDate;
            }

        }


        public string RunningTime
        {
            get
            {
                return this.m_RunningTime;
            }
        }


        public string WebPageUrl
        {
            get
            {
                return this.m_WebPageUrl;
            }
        }

        public override string ToString()
        {
            return String.Format("{0} {1}", this.m_Title, this.m_ReleasedDate);
        }

        /// <summary>
        /// 将 null 字符串转换为空字符串的实用工具函数。
        /// </summary>
        /// <param name="OriginalValue">检查其是否为 Nothing 的字符串。</param>
        /// <returns>如果原始值为 Nothing，则返回空字符串。否则，返回原始值。</returns>
        /// <remarks></remarks>
        private static string CheckString(string OriginalValue)
        {
            if (OriginalValue == null)
            {
                return "";
            }
            else
            {
                return OriginalValue;
            }
        }
    }
}



