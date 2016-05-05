# mysql-sharding
mysql的动态数据分片的设计
/*
 * ---  ShardGroup(0-10W] bInserted:true                    ----- ShardGroup(0w-10W] bInserted:false
 *      |                                                   ----- ShardGroup(10w-20W]bInserted:true
 *      |_ _ _ _ _ _ _ _ _
 *      |                 |
 * stShardGroupIntVal  stShard
 * (每个分组的RoleID范围                         |
 *  0-10W)                |(根据hash因子取余如 %2)      (扩容)=>
 *               _ _ _ _ _|_ _ _ _
 *              |(0)              |(1)
 *         stShardCell       stShardCell
 *                                |
 *                                |
 *                         stShardCellIntVal
 *                                |
 *                         _ _ _ _|_ _ _ _
 *                        |               |
 *                  细分数组范围(0-5W]  细分数组范围(5-10W]
 *
 */
