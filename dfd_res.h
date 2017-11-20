#ifndef DFD_RES
#define DFD_RES

/*nea domh thn opoia tha epistrefei h dfd_new adi gia ton double pou*/
/*epestrefe h dfd.Perilamvanei kai ton pinaka D ton opoio*/
/*xrismopoioume gia na vroume ta zeugaria simeiwn pou xeiazodai gia ton ypologismo*/
/*ths mdfc*/

struct dfd_res{
  int**D;
  int d1;
  int dist;
  dfd_res();
  dfd_res(int,int);
  ~dfd_res();
};

#endif
