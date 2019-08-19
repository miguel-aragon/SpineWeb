;
;
;
;
;
;
;

window, xs=1024,ys=1024,retain=2

PATH = 'SUB1/'

FILE = 'run_200'

FLAG_READ = 1
IF (FLAG_READ NE 0) THEN BEGIN
   den    = read_dvolume(PATH+FILE+'.dvol')
   wall   = read_ivolume(PATH+FILE+'.GAU20.iwall')
   fila   = read_ivolume(PATH+FILE+'.GAU20.ifila')
   stream0 = read_fvolume('SUB0/'+FILE+'.STREAMS.fvol')
   stream1 = read_fvolume('SUB1/'+FILE+'.STREAMS.fvol')
   stream2 = read_fvolume('SUB2/'+FILE+'.STREAMS.fvol')
ENDIF


;--- Get te wall/filament catalogues
walls = histogram(wall, min=0, REVERSE_INDICES=rev_w)
n_walls = n_elements(walls)
filas = histogram(fila, min=0, REVERSE_INDICES=rev_f)
n_filas = n_elements(filas)

;--- Collapsed objects
coll = dilata(stream0 GT 1,1)

wall_c = wall

cube = wall*0.0
FOR i=0L, n_walls-1 DO BEGIN

   ;--- Select one wall
   wall_i = rev_w[rev_w[i] : rev_w[i+1L]-1L]
   n_wall_i = walls[i]

   ;--- Streams intersecting the wall
   coll_i = coll[wall_i]
   n_collapsed = float(total(coll_i))
   IF (n_collapsed/n_wall_i LT 0.25) THEN wall_c[wall_i] = -1

   ;--- Density
   ;IF (mean(den[wall_i]) LE 0.25)     THEN wall_c[wall_i] = -1

ENDFOR








end

