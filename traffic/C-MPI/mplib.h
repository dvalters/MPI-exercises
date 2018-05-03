void mpstart(int *numproc, int *procid);
void mpstop(void);
void mpgsum(int *ival);
void mpscatter(int *inbuff, int *outbuff, int ndata);
void mpsendrecv(int *senddata, int sendproc, int *recvdata, int recvproc);
double gettime(void);
