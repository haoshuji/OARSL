
# Extract parameter sensitivity from _par.txt files into _OASIS.txt and _CWAS.txt files

import numpy as np
import matplotlib.pyplot as plt
import os.path

# loc = './arowd2/'
# loc = '/home/hsj/Dropbox/share(peilin-shuji)/IJCAI2015-AOS/figures/'
loc = './'
# loc = 'D:/Dropbox/projects/research/aos/results/arowf2/'
line_width = 2
marker_edge_width=[1,1,1,2.8,2.8,2.8,2.6,2.6,2.6]
label_size = 18
tick_size = 15
legend_size = 13
legend_font_size = 13
marker_size = 10

datas=['satimage','segment', 'letter', 'pendigits']

for data in datas:		
	data_fullname = loc+data+'_par.txt'	
	if os.path.isfile(data_fullname) == False:
		print data, 'does not exit'
		continue
	in_file = open(data_fullname,'r')
	in_file.readline()
	
	OASIS_C = [10**(-5+i) for i in range(11)]
	# print OASIS_C
	out_file = open(loc+data+'_OASIS.txt','w')
	OASIS_AP = []
	for i in range(len(OASIS_C)):
		line = in_file.readline().split()
		OASIS_AP.append(float(line[1]))
		out_file.write("%f " %float(line[1]))
	
	out_file.close()
	# print OASIS_AP

	out_file = open(loc+data+'_CWAS.txt','w')
	in_file.readline();in_file.readline();in_file.readline();in_file.readline()
	AROW_eta = [(-5+i) for i in range(11)]
	AROW_r = [(-5+i) for i in range(11)]
	# print AROW_eta, AROW_r
	AROW_AP = []
	for i in range(len(AROW_eta) * len(AROW_r)):
		line = in_file.readline().split()
		AROW_AP.append(float(line[2]))
		out_file.write("%f " %float(line[2]))
	in_file.close()
	out_file.close()

	# fig = plt.figure()
	# ax = fig.add_subplot(1,1,1)		
	# plt.plot(OASIS_C, OASIS_AP, 'ro',label = 'PAAS')
	# plt.xlabel('C',fontsize=label_size)
	# plt.ylabel('AP@10',fontsize=label_size)
	# ax.tick_params(axis='x', labelsize=tick_size)
	# ax.tick_params(axis='y', labelsize=tick_size)	
	# ax.set_xscale('log')
	# plt.legend(loc='best', ncol=1, shadow=True, fancybox=True,prop={'size':legend_size},fontsize=legend_font_size)
	# plt.grid(True,ls="--", color='0.4')
	# plt.savefig(loc+data+"_par_PAS_C.pdf")
	# plt.close(fig)	

	# from mpl_toolkits.mplot3d import Axes3D

	# fig = plt.figure()
	# ax = fig.add_subplot(111, projection='3d')	
	# for i in range(len(AROW_eta)):
	# 	for j in range(len(AROW_r)):			
	# 		ax.scatter(AROW_eta[i],AROW_r[j], AROW_AP[i*len(AROW_eta)+j],c='b',marker='s')
	# ax.set_xlabel('eta')
	# ax.set_ylabel('r')
	# ax.set_zlabel('AP@10')	
	# ax.tick_params(axis='x', labelsize=tick_size)
	# ax.tick_params(axis='y', labelsize=tick_size)	
	# # ax.w_xaxis.set_xscale('log')
	# # ax.w_yaxis.set_yscale('log')
	# # plt.legend(loc='best', ncol=1, shadow=True, fancybox=True,prop={'size':legend_size},fontsize=legend_font_size)
	# # labels = [item.get_text() for item in ax.get_xticklabels()]
	# # ax.set_xticklabels(['1e-05','1e-04','1e-03','1e-02','1e-01','1','1e+01','1e+02','1e+03','1e+04','1e+05'])
	# # ax.set_yticklabels(['1e-05','1e-04','1e-03','1e-02','1e-01','1','1e+01','1e+02','1e+03','1e+04','1e+05'])
	# # ax.w_xaxis.set_scale('symlog')
	# # ax.w_yaxis.set_scale('symlog')
	# plt.grid(True,ls="--", color='0.4')
	# # plt.show()
	# plt.savefig(loc+data+"_par_CWAS.pdf",format='pdf')
	# plt.close(fig)	


