#!/usr/bin/python
#
#
#  Run: python2.2 reduce2.py
#
#  Miguel Angel Aragon Calvo
#
#-----------------------------------------

from math import *
from time import strftime
from string import *
from os import execv
import string
import sys, os
from copy import copy

#=================================================
#--- Do median filtering and Gaussian smoothing
#=================================================
def clean_cube(cube_files, execute):


    print '----------------------------'
    print '>>> Cleaning image'
    print '----------------------------'
    print
    print 'Median filtering...'

    #--- MEDIAN FILTERING
    cont = 0
    for cube_file_i in cube_files:
        
        #--- Do median filtering
        command_line = 'median_filter ' + cube_file_i + ' 10 ' + cube_file_i.replace('.dvol','.GMF10.dvol')

        #--- Update filename
        cube_files[cont] = cube_file_i.replace('.dvol','.GMF10.dvol')

        #--- Execute command
        if execute == 1:
            os.system(command_line)
        else:
            print command_line

        cont = cont+1

    print
    print 'Gaussian smoothing...'

    #--- GAUSSIAN SMOOTHING
    cont = 0
    for cube_file_i in cube_files:

        #--- Gaussian smoothing
        command_line = 'fft_smooth ' + cube_file_i + ' 2.0 ' + cube_file_i.replace('.dvol','.GAU02.dvol')

        #--- Update filename
        cube_files[cont] = cube_file_i.replace('.dvol','.GAU02.dvol')
        
        #--- Execute command
        if execute == 1:
            os.system(command_line)
        else:
            print command_line

        cont = cont+1

    print
    print


#=================================================
#--- Watershed transform
#=================================================
def watershed_flood(cube_files, execute):

    print '----------------------------'
    print '>>> Incomplete watershed'
    print '----------------------------'

    #--- Do watershed
    cont = 0
    for cube_file_i in cube_files:

        #--- Watershed transform
        command_line = 'watershed_flood ' + cube_file_i + ' ' + cube_file_i.replace('.dvol','.ireg')

        #--- Update filename
        cube_files[cont] = cube_file_i.replace('.dvol','.ireg')

        #--- Execute command
        if execute == 1:
            os.system(command_line)
        else:
            print command_line

        cont = cont+1

    print
    print

                                                
#=================================================
#--- Hierarchical void merging
#=================================================
def merge_scales(cube_files, execute):

    print '----------------------------'
    print '>>> Merging adjacent scales'
    print '----------------------------'

    #--- Copy the files to use later
    cube_files_merged = cube_files[:]

    #--- Merge adjacent scales
    for i, cube_file_i in enumerate(cube_files):

        if (i == 0):  #--- Do for the first scale

            #--- Copy file for consistency with other scales
            command_line = 'cp ' + cube_file_i + ' ' + cube_files[0].replace('.ireg','.Y-0.imer')

            #--- This is the base name for the merged files
            base_hierarchy = cube_files[0].replace('.ireg','.Y-0.imer')

            #--- Store the merge filename
            cube_files_merged[i] = base_hierarchy

            #--- Execute command            
            if execute == 1:
                os.system(command_line)
            else:
                print command_line
           
        else:         #--- Rest of scales
            
            #--- Merge adjacent scales
            command_line = 'merge_voids_simple ' + cube_files_merged[i-1] + ' ' + cube_files[i] + ' ' + base_hierarchy.replace('.imer','-'+str(i)+'.imer')

            #--- Update file base of the hierarchy
            base_hierarchy = base_hierarchy.replace('.imer','-'+str(i)+'.imer')
            
            #--- Store the merge filename
            cube_files_merged[i] = base_hierarchy

            #--- Execute command
            if execute == 1:
                os.system(command_line)
            else:
                print command_line


    #--- Brute force copy. There must be better way...
    for i, cube_file_i in enumerate(cube_files):
        cube_files[i] = cube_files_merged[i]

    print
    print


#=================================================
#--- Do complete watershed transform
#=================================================
def complete_watershed(file_densi, cube_files, execute):


    print '----------------------------'
    print '>>> Complete watershed'
    print '----------------------------'

    #--- Merge adjacent scales
    for i, cube_file_i in enumerate(cube_files):

        #--- Copy file for consistency with other scales
        command_line = 'watershed_flood_fix_spine ' + cube_file_i + ' ' + file_densi + ' ' + cube_file_i.replace(".imer", ".iwat")

        #--- Update filenames
        cube_files[i] = cube_file_i.replace(".imer", ".iwat")

        #--- Execute command
        if execute == 1:
            os.system(command_line)
        else:
            print command_line
            
#=================================================
#--- Compute spine from watershed
#=================================================
def get_spine(cube_files, execute):


    print '----------------------------'
    print '>>> Get spine'
    print '----------------------------'

    #--- Merge adjacent scales
    for i, cube_file_i in enumerate(cube_files):

        #--- Copy file for consistency with other scales
        command_line = 'watershed_to_spine ' + cube_file_i + ' ' + cube_file_i.replace(".iwat", ".cspi")

        #--- Execute command
        if execute == 1:
            os.system(command_line)
        else:
            print command_line





#===============================================================
#                         MAIN
#===============================================================
n_snaps = 1

#--- Loop over snapshots for the three scales
for i in range(n_snaps):

    path1 = "./"
    path2 = "./"
    path3 = "./"
    file1 = "MMF200M.512.dvol"
    file2 = "MMF200L.512.dvol"
    file3 = "MMF200S.512.dvol"

    #--- These are the files to process
    data = [path1+file1, path2+file2, path3+file3]

    execute = 0
    clean_cube(data,execute)

    #--- Use this density field as base to merge the watersheds
    file_densi = data[0]
    
    execute = 0
    watershed_flood(data,execute)

    execute = 0
    merge_scales(data,execute)

    execute = 0
    complete_watershed(file_densi, data, execute)

    execute = 0
    get_spine(data, execute)

    print
    print
    print '====================== ' + str(i) + ' ==========================='


print
print
print '>>> DONE !!! '
print
print

