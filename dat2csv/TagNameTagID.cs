using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace dat2csv
{
    /// <summary>
    /// 点名点号
    /// </summary>
    public class TagNameTagID:IComparable<TagNameTagID>
    {
        public const int Size = 14;
        public string TagName { get; set; }
        public int TagID { get; set; }

        public int CompareTo(TagNameTagID other)
        {

            return this.TagID - other.TagID;

        }
    }
}
