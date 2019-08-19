;
;
;
;
;
;
window, xs=1024,ys=1024,retain=2

ng = 256L
np = ng^3

;--- Create grid
;tr   = lonarr(4, 6L*(ng)^3) - 999
;tr_d = fltarr(6L*(ng)^3)    - 999
;get_lagrangian_tetrahedra, ng, tr
;save, tr, filename='TR_256.LTFE'
restore, '/media/miguel/DATA1/Bolshoi/DENSITY/TR_256.LTFE'

PATH_GAD = 'SUB3/'

FILE = 'run_200'

;--- Sort indexes
read_snap_all_manual_fix, PATH_GAD + FILE, PATH_GAD + FILE, /SORTED
;--- Read positions
snap = read_snap_all_manual(PATH_GAD + FILE,/POSI)        
box  = snap.boxsize
x = reform(snap.pos[0,*])
y = reform(snap.pos[1,*])
z = reform(snap.pos[2,*])

;--- Compute densities with one orientation
;SPAWN, ['tetraden', PATH_GAD + FILE, string2(ng)],/NOSHELL
;den_tr = read_gauss_density(PATH_GAD + FILE + '.tden')

;--- Compute densities with all orientations
SPAWN, ['tetraden_all', PATH_GAD + FILE, string2(ng)],/NOSHELL
deni = read_gauss_density(PATH_GAD + FILE + '.all.lden')
;--- Mean density inside tetrahedron
den_tr = reform((deni[tr[0,*]] + deni[tr[1,*]] + deni[tr[2,*]] + deni[tr[3,*]] ) / 4.0)

;--- Compute density fields
print, '>>> rasterizing...'
n_grid = 512L
xg = x/box*n_grid
yg = y/box*n_grid
zg = z/box*n_grid
den  = raster_tetrahedra_indexed_flat_lib(tr, xg,yg,zg, 1./den_tr, n_grid,n_grid,n_grid)

den = double(fill_zeroes_3d(den,/FIX_NAN))
den = double(den)
write_dvolume, den, n_grid, n_grid,n_grid,n_grid, 0,0,0,  PATH_GAD + FILE + '.dvol'

den = smooth_3d(den, 2.0, /DOUBLE)
write_dvolume, den, n_grid, n_grid,n_grid,n_grid, 0,0,0,  PATH_GAD + FILE + '.GAU20.dvol'

;--- Compute streams
streams = raster_tetrahedra_indexed_flat_lib(tr, xg,yg,zg, den_tr*0+1, n_grid,n_grid,n_grid)
streams = double(fill_zeroes_3d(streams,/FIX_NAN))
write_fvolume, streams, n_grid, n_grid,n_grid,n_grid, 0,0,0,  PATH_GAD + FILE + '.STREAMS.fvol'

;--- Spine fields
SPAWN, ['watershed_sort',            PATH_GAD + FILE + '.GAU20.dvol', PATH_GAD + FILE + '.GAU20.iwat'], /NOSHELL

SPAWN, ['watershed_to_spine',        PATH_GAD + FILE + '.GAU20.iwat', PATH_GAD + FILE + '.GAU20.cspi'], /NOSHELL

SPAWN, ['spine_wall_catalogues',     PATH_GAD + FILE + '.GAU20.iwat', PATH_GAD + FILE + '.GAU20.cspi', PATH_GAD + FILE + '.GAU20.iwall'], /NOSHELL

SPAWN, ['spine_filament_catalogues', PATH_GAD + FILE + '.GAU20.iwat', PATH_GAD + FILE + '.GAU20.cspi', PATH_GAD + FILE + '.GAU20.ifila'], /NOSHELL



end
