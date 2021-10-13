type PostConnAcceptPlugin

type PostConnAcceptPlugin interface{
	HandleConnAccept(net.Conn) (net.Conn,bool)
}

type PostReadRequestPlugin

type PostReadRequestPlugin interface{
	PostReadRequestPlugin(ctx context,Context,r *protocol.Message, e error) error
}

