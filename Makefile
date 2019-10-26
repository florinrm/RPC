build:
	rpcgen -C filewords.x
	g++ -o fileserver server.cpp filewords_svc.c filewords_xdr.c -lnsl
	g++ -o fileclient client.cpp filewords_clnt.c filewords_xdr.c -lnsl

clean:
	rm -f fileclient fileserver filewords_* filewords_* filewords.h