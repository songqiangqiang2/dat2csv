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

            public static void ConvertToCsv(string dataFilePath)
            {

                if (!File.Exists(dataFilePath))
                {
                    throw new FileNotFoundException(dataFilePath);
                }

                DatFileHead head;

                #region 这些信息相当于成员变量了都
                //dat文件的字节缓冲区
                byte[] datFileBuffer;

                //点名点号list
                List<TagNameTagID> tagNameTagIDs;

                //搜集周期类型信息
                CollectInfo[] collectInfos;

                //搜集索引表在dat中的偏移
                int indexStartPos = 0;


                //一个历史点信息的大小
                const int SizeOfOneHistAnaTagValue = 5;
                #endregion

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

                        //获取所有点名点号
                        int allTagCount = head.DDLength / TagNameTagID.Size;
                        tagNameTagIDs = new List<TagNameTagID>(allTagCount);

                        ms.Position = head.DDOffset;
                        //读取点名点号列表
                        for (int i = 0; i < allTagCount; i++)
                        {
                            byte[] tagNameBytes_gb2312 = br.ReadBytes(12);
                            string tagName = _encoding_gb2312.GetString(tagNameBytes_gb2312);
                            tagName = tagName.Trim(new char[] { '\0' });
                            int tagID = br.ReadInt16();
                            tagNameTagIDs.Add(new TagNameTagID { TagName = tagName, TagID = tagID });
                        }

                        //将点名点号按照点号进行排序
                        tagNameTagIDs.Sort();

                        //搜集索引表偏移
                        indexStartPos = head.IndexOffset;
                        ms.Position = indexStartPos;
                        //1.获取搜集周期个数
                        int diffPeriodCount = br.ReadInt16();

                        //将流定位到搜集类型信息处
                        ms.Position += 8;

                        collectInfos = new CollectInfo[5];

                        #region 填充各个搜集周期信息
                        for (int i = 0; i < diffPeriodCount; i++)
                        {
                            collectInfos[i] = new CollectInfo();
                            //1.搜集周期
                            short period = br.ReadInt16();
                            collectInfos[i].Period = period;


                            //2.此周期包含的组数
                            short grpCount = br.ReadInt16();
                            collectInfos[i].GroupCount = grpCount;

                            //3.十分钟点个数
                            int tagCountInTenMinutes = br.ReadInt32();
                            collectInfos[i].TagCountInTenMinutes = tagCountInTenMinutes;

                            //4.组信息偏移
                            int grpInfoOffset = br.ReadInt32();
                            collectInfos[i].GroupInfoOffset = grpInfoOffset;

                            //5.
                            ms.Position += 2;


                        }


                        //遍历各个搜集周期
                        for (int i = 0; i < diffPeriodCount; i++)
                        {

                            //遍历搜集周期中的各个组信息,并以第一组信息为出发点对数据进行搜集
                            for (int j = 0; j < collectInfos[i].GroupCount; j++)
                            {
                                short secondOffset = br.ReadInt16();
                                short maxTagCount = br.ReadInt16();
                                short currentTagCount = br.ReadInt16();
                                short initTagCount = br.ReadInt16();

                                int tagIDOffset = br.ReadInt32();
                                int tagDataOffset = br.ReadInt32();
                                ms.Position += 6;

                                //以第一组搜集信息为出发点进行搜集
                                if (secondOffset == 1)
                                {
                                    collectInfos[i].MaxTagCount = maxTagCount;
                                    collectInfos[i].CurrentTagCount = currentTagCount;
                                    collectInfos[i].TagIDOffset = tagIDOffset;
                                    collectInfos[i].TagDataOffset = tagDataOffset + head.DataOffset;
                                }
                                else
                                {
                                    collectInfos[i].MaxTagCount += maxTagCount;
                                    collectInfos[i].CurrentTagCount += currentTagCount;
                                    //collectInfos[i].TagIDOffset = tagIDOffset;
                                    //collectInfos[i].TagDataOffset = tagDataOffset;
                                }

                            }
                        }



                        //填充点号信息
                        for (int i = 0; i < diffPeriodCount; i++)
                        {
                            int tagCount = collectInfos[i].CurrentTagCount;
                            collectInfos[i].TagIDs = new List<int>(tagCount);

                            ms.Position = indexStartPos + collectInfos[i].TagIDOffset;
                            //读取需要搜集点的点号
                            for (int j = 0; j < tagCount; j++)
                            {
                                int id = br.ReadInt16();
                                collectInfos[i].TagIDs.Add(id);
                            }

                        }


                        #endregion

                        //准备读取需要读取的历史点号

                        #region 读取历史数据，每个搜集周期创建一个文件

                        for (int periodIndex = 0; periodIndex < diffPeriodCount; periodIndex++)
                        {
                            string fileName = Path.GetFileNameWithoutExtension(dataFilePath);
                            fileName += "_";
                            fileName += collectInfos[periodIndex].Period.ToString();
                            fileName += ".csv";
                            using (FileStream fs = new FileStream(fileName, FileMode.Create, FileAccess.ReadWrite))
                            {
                                using (StreamWriter sw = new StreamWriter(fs))
                                {
                                    //写文件头,列举点名
                                    sw.Write(@"时间/点名");
                                    for (int i = 0; i < collectInfos[periodIndex].TagIDs.Count; i++)
                                    {
                                        int tid = collectInfos[periodIndex].TagIDs[i];
                                        int index = tagNameTagIDs.BinarySearch(new TagNameTagID { TagID = tid });
                                        sw.Write("," + tagNameTagIDs[index].TagName);
                                    }
                                    sw.Write(Environment.NewLine);//回车

                                    

                                    //遍历十分钟数据内的每个点
                                    int tagCountInTenMinutes = collectInfos[periodIndex].TagCountInTenMinutes;
                                    for (int tagIndex = 0; tagIndex < tagCountInTenMinutes; tagIndex++)
                                    {
                                        //搜集每个点前，先定位到其在数据区的首地址
                                        ms.Position = collectInfos[periodIndex].TagDataOffset+SizeOfOneHistAnaTagValue*tagIndex;


                                        sw.Write(DateTime.Now.ToShortTimeString());

                                        //遍历
                                        for (int Index = 0; Index < collectInfos[periodIndex].CurrentTagCount; Index++)
                                        {
                                            float value = br.ReadSingle();
                                            sw.Write(",");
                                            sw.Write(value);
                                            ms.Position += 1;//5字节中，前四个字节表示AV，最后一个字节表示历史值的状态

                                            //定位到这个点的下一个存储位置
                                            ms.Position -= 5;
                                            ms.Position += SizeOfOneHistAnaTagValue * tagCountInTenMinutes;
                                        }
                                        sw.Write(Environment.NewLine);
                                    }

                                }//end sw
                            }//end fs

                        }//end for write a file
                        
                        #endregion

                    }

                }


            }
        }

        static void Main(string[] args)
        {

            Dat2CsvConverter.ConvertToCsv("62.dat");

            //converter.

        }
    }
}
