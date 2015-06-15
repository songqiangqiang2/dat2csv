using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace dat2csv
{
    

    class DatFileHead
    {
        public int Version { get; set; }
        public int DDOffset { get; set; }
        public int DDLength { get; set; }
        public int IndexOffset { get; set; }
        public int IndexLength { get; set; }
        public int DataOffset { get; set; }
        public int DataLength { get; set; }
    }
}
