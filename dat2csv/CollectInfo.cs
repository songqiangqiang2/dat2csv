using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace dat2csv
{

    /// <summary>
    /// 表示一个搜集周期类型所需要的信息
    /// </summary>
    public class CollectInfo
    {

        /// <summary>
        /// 搜集周期
        /// </summary>
        public int Period { get; set; }


        /// <summary>
        /// 此搜集周期包含的组个数
        /// </summary>
        public int GroupCount { get; set; }

        /// <summary>
        /// 此搜集周期十分钟内可以搜集的点个数e.g. 600/300/200
        /// </summary>
        public int TagCountInTenMinutes { get; set; }


        /// <summary>
        /// 搜集周期信息中第一组信息的偏移
        /// </summary>
        public int GroupInfoOffset { get; set; }



        /// <summary>
        /// 此搜集周期类型需要搜集的点个数(理论上限)
        /// </summary>
        public int MaxTagCount { get; set; }


        /// <summary>
        /// 此搜集周期类型需要搜集的点个数(实际)
        /// </summary>
        public int CurrentTagCount { get; set; }


        /// <summary>
        /// 此搜集周期类型第一个点在dat文件中的索引
        /// </summary>
        public int TagIDOffset { get; set; }


        /// <summary>
        /// 此搜集周期类型需要搜集点的点号数组
        /// </summary>
        public List<int> TagIDs { get; set; }



        /// <summary>
        /// 此搜集周期类型第一个点在dat文件中的偏移
        /// </summary>
        public int TagDataOffset { get; set; }


    }
}
