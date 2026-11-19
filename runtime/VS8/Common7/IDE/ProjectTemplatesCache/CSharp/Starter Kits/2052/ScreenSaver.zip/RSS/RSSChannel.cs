using System;
using System.Collections.Generic;
using System.Xml;

namespace $safeprojectname$.Rss
{
    /// <remarks>
    /// 表示 RSS 2.0 XML 文档中的一个 Channel 元素。
    /// 一个 RssFeed 中包含一个或多个 RssChannel。
    /// </remarks>
    public class RssChannel
    {
        private readonly string title;
        private readonly string link;
        private List<RssItem> items;

        public string Title { get { return title; } }
        public string Link { get { return link; } }
        public IList<RssItem> Items { get { return items.AsReadOnly(); } }

        /// <summary>
        /// 从一个表示 RSS 2.0 XML 文档内的 Channel 元素的 XmlNode 生成一个 RSSChannel。
        /// </summary>
        /// <param name="channelNode"></param>
        internal RssChannel(XmlNode channelNode)
        {
            items = new List<RssItem>();
            title = channelNode.SelectSingleNode("title").InnerText;
            link = channelNode.SelectSingleNode("link").InnerText;

            XmlNodeList itemNodes = channelNode.SelectNodes("item");
            foreach (XmlNode itemNode in itemNodes)
            {
                items.Add(new RssItem(itemNode));
            }
        }
    }
}
