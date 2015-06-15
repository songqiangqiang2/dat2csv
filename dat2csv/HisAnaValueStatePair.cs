using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace dat2csv
{
    /// <summary>
    /// 历史数据【值/状态】结构
    /// </summary>
    public struct HisAnaValueStatePair
    {
        public float Value { get; set; }
        public byte State { get; set; }
    }
}
