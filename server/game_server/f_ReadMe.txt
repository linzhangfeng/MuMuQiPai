1 /create_room
请求参数：
{
    userId //用户id
}

返回参数：
{
	status      //返回状态 200,
	errmsg      //返回错误信息"ok",
	roomId      //房间id,
	sign        //签名
}

////////////////////////////////////////////////
2 /check_room
请求参数：
{
    userId //用户id
}

返回参数
{
    status      //返回状态 200,
	errmsg      //回错误信息,
	roomId      //房间ID,
	isRuning    //房间是否解散                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
	sign        //签名
}

////////////////////////////////////////////////
3 /get_room_info
请求参数：
{
    userId //用户id
}

返回参数
{
    status      //返回状态 200,
	errmsg      //回错误信息,
	roomId      //房间id,
	roomData    //房间数据,                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
	sign        //签名
}

////////////////////////////////////////////////
4 /add_room
请求参数：
{
    roomId //放假id
    userId //用户id
}

返回参数
{
    status      //返回状态 200,
	errmsg      //回错误信息,
	roomId      //房间id,
	sign        //签名
}