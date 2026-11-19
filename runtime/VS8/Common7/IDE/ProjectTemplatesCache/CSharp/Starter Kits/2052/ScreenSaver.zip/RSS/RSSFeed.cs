using System;
using System.Collections.Generic;
using System.IO;
using System.Net;
using System.Xml;

namespace $safeprojectname$.Rss
{
    /// <summary>
    /// 表示 RSS 2.0 XML 文档中的一个 RSS 元素。
    /// </summary>
    public class RssFeed
    {
        private List<RssChannel> channels;
        public IList<RssChannel> Channels { get { return channels.AsReadOnly(); } }
        public RssChannel MainChannel { get { return Channels[0]; } }

        /// <summary>
        /// 以工厂模式使用的私有构造函数。
        /// </summary>
        /// <param name="xmlNode">一个包含 RSSFeed 内容的 XML 块。</param>
        private RssFeed(XmlNode xmlNode)
        {
            channels = new List<RssChannel>();

            // 读取 <rss> 标记
            XmlNode rssNode = xmlNode.SelectSingleNode("rss");

            // 为 <rss> 节点中的每个 <channel> 节点
            // 添加一个频道。
            XmlNodeList channelNodes = rssNode.ChildNodes;
            foreach (XmlNode channelNode in channelNodes)
            {
                RssChannel newChannel = new RssChannel(channelNode);
                channels.Add(newChannel);
            }
        }
        
        /// <summary>
        /// 根据一个指向有效的 RSS 2.0 XML 文件的 URI 构造 RSSFeed 对象的工厂。
        /// </summary>
        /// <exception cref="System.Net.WebException">当无法在 Web 上找到该 URI 时引发。</exception>
        /// <param name="uri">从该处读取 RSS 源的 URI。</param>
        public static RssFeed FromUri(string uri)
        {
            XmlDocument xmlDoc;
            WebClient webClient = new WebClient();
            using (Stream rssStream = webClient.OpenRead(uri))
            {
                TextReader textReader = new StreamReader(rssStream);
                XmlTextReader reader = new XmlTextReader(textReader);
                xmlDoc = new XmlDocument();
                xmlDoc.Load(reader);
            }
            return new RssFeed(xmlDoc);
        }

        /// <summary>
        /// 根据一个 RSS 2.0 XML 文件的文本构造 RssFeed 对象的工厂。
        /// </summary>
        /// <param name="rssText">一个包含 RSS 源的 XML 数据的字符串。</param>
        public static RssFeed FromText(string rssText)
        {
            XmlDocument xmlDoc = new XmlDocument();
            xmlDoc.LoadXml(rssText);
            return new RssFeed(xmlDoc);
        }
    }
}