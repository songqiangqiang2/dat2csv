using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace dat2csv
{
    class Program
    {

        public class Dat2CsvConverter
        {



            static Encoding _encoding_gb2312 = Encoding.GetEncoding(936);
            const int TAG_COUNT_IN_10_MINUTES = 600;


            public static void ConvertToCsv(string dataFilePath)
            {

                if (!File.Exists(dataFilePath))
                {
                    throw new FileNotFoundException(dataFilePath);
                }


                DatFileHead head;

                //dat文件的字节缓冲区
                byte[] datFileBuffer;

                //点名点号list
                List<TagNameTagID> tagNameTagIDs;

                //实际需要获取的点号数组
                int[] tagIDsNeedToRead;


                //to do获取dat所对应的时间
                DateTime time = DateTime.Now;





                datFileBuffer = File.ReadAllBytes(dataFilePath);
                using (MemoryStream ms = new MemoryStream(datFileBuffer, false))
                {
                    using (BinaryReader br = new BinaryReader(ms))
                    {
                        head = new DatFileHead();
                        head.Version = br.ReadInt32();
                        head.DDOffset = br.ReadInt32();
                        head.DDLength = br.ReadInt32();
                        head.IndexOffset = br.ReadInt32();
                        head.IndexLength = br.ReadInt32();
                        head.DataOffset = br.ReadInt32();
                        head.DataLength = br.ReadInt32();

                        //计算dat中需要搜集的点个数
                        int tagCount = head.DDLength / TagNameTagID.Size;
                        tagNameTagIDs = new List<TagNameTagID>(tagCount);


                        //读取点名点号列表
                        for (int i = 0; i < tagCount; i++)
                        {
                            byte[] tagNameBytes_gb2312 = br.ReadBytes(12);
                            string tagName = _encoding_gb2312.GetString(tagNameBytes_gb2312);
                            tagName = tagName.Trim(new char[] { '\0' });
                            int tagID = br.ReadInt16();
                            tagNameTagIDs.Add(new TagNameTagID { TagName = tagName, TagID = tagID });
                        }

                        //将点名点号按照点号进行排序
                        tagNameTagIDs.Sort();


                        //准备读取需要读取的历史点号

                        #region 当前版本做的比较差，只获取了最特殊最常用的情况，就是只有一个收集周期，且都是1秒


                        int diffCollectTypeNumber = br.ReadInt16();
                        if (diffCollectTypeNumber != 1)
                        {
                            throw new IndexOutOfRangeException("当前程序版本无法解析此dat文件");
                        }

                        int tmpOffset = 0;
                        int tmpNumber = 0;
                        ms.Position += 4;
                        tmpOffset = br.ReadInt32();
                        if (tmpOffset != 10)
                        {
                            throw new IndexOutOfRangeException("当前程序版本无法解析此dat文件");
                        }



                        //1
                        tmpNumber = br.ReadInt16();
                        if (tmpNumber != 1)
                        {
                            throw new IndexOutOfRangeException("当前程序版本无法解析此dat文件");
                        }
                        //1
                        tmpNumber = br.ReadInt16();
                        if (tmpNumber != 1)
                        {
                            throw new IndexOutOfRangeException("当前程序版本无法解析此dat文件");
                        }

                        //600
                        tmpNumber = br.ReadInt32();
                        if (tmpNumber != 600)
                        {
                            throw new IndexOutOfRangeException("当前程序版本无法解析此dat文件");
                        }

                        //24
                        tmpOffset = br.ReadInt32();
                        if (tmpOffset != 24)
                        {
                            throw new IndexOutOfRangeException("当前程序版本无法解析此dat文件");
                        }

                        ms.Position += 2;

                        tmpNumber = br.ReadInt16();
                        if (tmpNumber != 1)
                        {
                            throw new IndexOutOfRangeException("当前程序版本无法解析此dat文件");
                        }

                        int maxTagCountInDatFile = br.ReadInt16();
                        int currentTagCountNeedToCollect = br.ReadInt16();
                        if (currentTagCountNeedToCollect > maxTagCountInDatFile)
                        {
                            throw new IndexOutOfRangeException("当前程序版本无法解析此dat文件");
                        }

                        int initTagCountNeedToCollect = br.ReadInt16();
                        if (initTagCountNeedToCollect != currentTagCountNeedToCollect)
                        {
                            throw new IndexOutOfRangeException("当前程序版本无法解析此dat文件");
                        }

                        #endregion



                        int RealTagIndexInDat = br.ReadInt32();
                        RealTagIndexInDat += head.IndexOffset;
                        //将流定为到实际需要搜集的点号区
                        ms.Position = RealTagIndexInDat;


                        tagIDsNeedToRead = new int[currentTagCountNeedToCollect];
                        for (int i = 0; i < currentTagCountNeedToCollect; i++)
                        {
                            int tagID = br.ReadInt16();
                            tagIDsNeedToRead[i] = tagID;
                        }


                        //写文件
                        using (FileStream fs = new FileStream(Path.ChangeExtension(dataFilePath, "csv"), FileMode.Create, FileAccess.ReadWrite))
                        {
                            using (StreamWriter sw = new StreamWriter(fs))
                            {
                                //写文件头
                                sw.Write(@"时间\点名");
                                for (int i = 0; i < tagIDsNeedToRead.Length; i++)
                                {
                                    int tid = tagIDsNeedToRead[i];

                                    int index = tagNameTagIDs.BinarySearch(new TagNameTagID { TagID = tid });
                                    if (index < 0)
                                    {
                                        throw new IndexOutOfRangeException("oops");
                                    }


                                    sw.Write("," + tagNameTagIDs[index].TagName);
                                }
                                sw.Write(Environment.NewLine);//回车



                                //将流定位到实际数据区
                                ms.Position = head.DataOffset;
                                int pos_DataStart = (int)ms.Position;

                                //to do 加注释
                                int pos = 0;
                                int pos2 = 0;

                                //每个历史值占用的字节数
                                const int sizeOfOneHistAnaValue = 5;
                                //每两个不同点之间的字节间隔
                                const int DistanceBetweenTwoTag = sizeOfOneHistAnaValue * TAG_COUNT_IN_10_MINUTES;//600*5


                                for (int i = 0; i < TAG_COUNT_IN_10_MINUTES; i++)//600
                                {
                                    //
                                    pos = sizeOfOneHistAnaValue * i + pos_DataStart;

                                    sw.Write(time);
                                    //遍历所有需要搜集的点
                                    for (int j = 0; j < tagIDsNeedToRead.Length; j++)
                                    {
                                        pos2 = pos + DistanceBetweenTwoTag * j;
                                        ms.Position = pos2;
                                        float value = br.ReadSingle();

                                        byte status = br.ReadByte();

                                        sw.Write(",");
                                        sw.Write(value);//需要调整下格式
                                    }

                                    time = time.AddSeconds(1.0);
                                    sw.Write(Environment.NewLine);


                                }
                            }



                        }


                    }

                }


            }
        }

        static void Main(string[] args)
        {

            Dat2CsvConverter.ConvertToCsv("10.dat");

            //converter.

        }
    }
}
