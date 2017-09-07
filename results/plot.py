
# coding: utf-8

# In[60]:

import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt
import os.path

mpl.rcParams['ps.fonttype']=42 # avoid using type 3 fonts
mpl.rcParams['pdf.fonttype']=42 # avoid using type 3 fonts

line_width = 2
num_alg = 7
marker_edge_width=[1,1,1,2.8,2.8,2.8,2.6,2.6,2.6]
label_size = 18
tick_size = 15
legend_size = 17
legend_font_size = 20
marker_size = 10

algorithm_order = [0,6,3,2,5,1,4]
acc=[6,8,10]

lines_each_algorithm = 12

# Data
colors = ['blue', 'red', 'green', 'blue', 'red', 'green','blue', 'magenta']
markers = ['^', 's', 'o', '^', 's', 'o','x', 'd']
filltypes = ['full','full','full','none','none','none','none','none']
ls = ['--','--','--','-','-','-','-','-',]
x_axis_label='Varied Query Ratio'
y_axis_label_list=['AP@10','mAP@10','Time(s)']
y_axis_label_list_key = ['AP_test', 'mAP_test', 'time']
output_file_type = '.pdf'

file_names = ['_AP_Test', '_mAP_Test', '_time']
step_size = 2

input_loc = './0301/'
output_loc = input_loc

all_data = ['shuttle_scale_all','segment_scale', 'satimage_scale_all', 'pendigits_all', 'letter_scale_all','covtype' ]
all_data = ['letter','pendigits','satimage','segment','shuttle']
all_data = ['covtype']
for data in all_data:	
	for norm_method in ['_length1']: #['_orignal','_length1','_scale01','_gaussian']:
		input_data_fullname = input_loc+data+norm_method
		print input_data_fullname
		if os.path.isfile(input_data_fullname+'.txt') == False:
			print data, 'does not exit'
			continue

		_diagonal_version = False
		_algorithm_names=[]
		if data in ['caltech50', 'caltech249','caltech20', 'caltech10','aloi50','mnist', 'aloi10']:
			_algorithm_names = ['OASIS','PAAS','PARS', 'CWS-d', 'CWAS-d', 'CWRS-d','AROMA-d','CWAS_Adap-d']
			_diagonal_version = True
		else:
			_algorithm_names = ['OASIS','PAAS','PARS', 'CWS', 'CWAS', 'CWRS', 'AROMA','CWAS_Adap']
		_algorithm_plot_order_index=[0,3,6,2,5,1,4,7]

		# read result into dictionary structure
		_results_dict = {}
		with open(input_data_fullname+'.txt') as _fin:
			for line in _fin:
				_str_list = line.strip().split()
				
				if len(_str_list) == 2:
					_str_list_last = _str_list
					if _str_list[0] not in _results_dict:
						_results_dict[_str_list[0]] = {}
						_results_dict[_str_list[0]][_str_list[1]]=[]
					else:
						_results_dict[_str_list[0]][_str_list[1]]=[]
				else:
					# print _str_list
					_float_list = [float(x) for x in _str_list]
					_results_dict[_str_list_last[0]][_str_list_last[1]] = _float_list
		
		if _diagonal_version == True:
			for passive_algorithm in ['OASIS','CWS-d','AROMA-d']:
				_num_query_ratio = len(_results_dict[passive_algorithm]['que'])
				_results_dict[passive_algorithm]['que']=np.linspace(0,1,_num_query_ratio)
		else:
			for passive_algorithm in ['OASIS','CWS','AROMA']:
				_num_query_ratio = len(_results_dict[passive_algorithm]['que'])
				_results_dict[passive_algorithm]['que']=np.linspace(0,1,_num_query_ratio)

		for _metric_index in range(len(y_axis_label_list)):
			_y_axis_label = y_axis_label_list[_metric_index]
			_y_axis_key = y_axis_label_list_key[_metric_index]
			fig = plt.figure()
			ax = fig.add_subplot(1,1,1)		
			for _alg_index in _algorithm_plot_order_index:
				# print _alg_index
				plt.plot(_results_dict[_algorithm_names[_alg_index]]['que'], _results_dict[_algorithm_names[_alg_index]][_y_axis_key], \
							label = _algorithm_names[_alg_index], ls=ls[_alg_index], color=colors[_alg_index], marker = markers[_alg_index],\
							fillstyle=filltypes[_alg_index],markersize=marker_size,mew=marker_edge_width[_alg_index])	
				if data in ['satimage','segment', 'letter','pendigits']:
					start, end = ax.get_ylim()
					ax.yaxis.set_ticks(np.arange(0.0, end+0.15, 0.1))
					ax.xaxis.set_ticks(np.arange(0, 1.2, 0.2))
			plt.xlabel(x_axis_label,fontsize=label_size)
			plt.ylabel(_y_axis_label,fontsize=label_size)
			ax.tick_params(axis='x', size=tick_size)
			ax.tick_params(axis='y', size=tick_size)
			if 'caltech' in data:
				plt.legend(loc='best', ncol=1, shadow=True, fancybox=True,prop={'size':legend_size},fontsize=legend_font_size)
			else:	
				plt.legend(loc='best', ncol=2, shadow=True, fancybox=True,prop={'size':legend_size},fontsize=legend_font_size)
			plt.grid(True,which="both",ls="--", color='0.4')
			if _diagonal_version:
				plt.savefig(output_loc+data+norm_method+'_'+_y_axis_key+'_diag'+output_file_type)
			else:
				plt.savefig(output_loc+data+norm_method+'_'+_y_axis_key+output_file_type)
			plt.close(fig)	
	# # query ratio vs performance
	# for t in range(len(file_names)):
	# 	start=0
	# 	fig = plt.figure()
	# 	ax = fig.add_subplot(1,1,1)		
	# 	queBaseAlg = np.zeros(d)	
	# 	for i in algorithm_order:	
	# 		plt.plot(M[i*lines_each_algorithm,0:20:step_size], M[i*lines_each_algorithm+acc[t],0:20:step_size], \
	# 					label = algorithm_names[i], ls=ls[i], color=colors[i], marker = markers[i],\
	# 					fillstyle=filltypes[i],markersize=marker_size,mew=marker_edge_width[i])			
	# 	if data in ['satimage','segment', 'letter','pendigits']:
	# 		start, end = ax.get_ylim()
	# 		ax.yaxis.set_ticks(np.arange(0.0, end+0.15, 0.1))
	# 		ax.xaxis.set_ticks(np.arange(0, 1.2, 0.2))
	# 	plt.xlabel(x_algorithm_names[t],fontsize=label_size)
	# 	plt.ylabel(y_algorithm_names[t],fontsize=label_size)
	# 	ax.tick_params(axis='x', algorithm_namesize=tick_size)
	# 	ax.tick_params(axis='y', algorithm_namesize=tick_size)
	# 	# plt.gca().get_xaxis().get_major_formatter().set_useOffset(False)
	# 	if 'Log' in file_names[t]:
	# 		ax.set_xscale('log')
	# 	if 'caltech' in data:
	# 		plt.legend(loc='best', ncol=1, shadow=True, fancybox=True,prop={'size':legend_size},fontsize=legend_font_size)
	# 	else:	
	# 		plt.legend(loc=4, ncol=2, shadow=True, fancybox=True,prop={'size':legend_size},fontsize=legend_font_size)
	# 	plt.grid(True,which="both",ls="--", color='0.4')
	# 	plt.savefig(output_loc+data+file_names[t]+output_file_type)
	# 	plt.close(fig)	

	