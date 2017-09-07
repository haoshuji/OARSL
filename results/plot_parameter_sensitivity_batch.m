dataset={'satimage','segment', 'letter', 'pendigits'} 
% dataset = {'letter'}
input_loc = './5315/';
output_loc = 'C:\users\hsj\Dropbox\share(peilin-shuji)\AOS-CIKM15\figures\parameters\';
% input_loc = output_loc;
for i=1:length(dataset),
    display(dataset{i});
    plot_parameter_sensitivity(input_loc,output_loc,dataset{i});
end
