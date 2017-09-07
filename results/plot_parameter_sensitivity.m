    function plot_parameter_sensitivity(input_loc,output_loc,dataset)
    parameters = [10^-5, 10^-4,10^-3,10^-2,10^-1,10^0, 10^1,10^2,10^3,10^4,10^5];
    parameters2 = [10^-5,  1,  10^5];
    parameters3 = [10^-5,  1,  10^5];
    num_par = length(parameters)
    
%     fid = fopen(strcat(input_loc, dataset, '_par_figure.txt'));
%     tline = fgets(fid);
%     C = strsplit(tline);
%     
%     OASIS = zeros(num_par,1);
%     for i=1:num_par,
%         C(i)
%         OASIS(i) = str2double(C(i));
%     end
%     
%     tline = fgets(fid);
%     C = strsplit(tline);
%     AROW= zeros(num_par*num_par,1);    
%     for i=1:(num_par*num_par),
%         AROW(i) = str2double(C(i));
%     end
	OASIS = dlmread(strcat(input_loc,dataset,'_OASIS.txt'));
    AROW = dlmread(strcat(input_loc, dataset, '_AROW.txt'));
   
    
    legendsize=10;
    labelsize=32;
    linewidth=2;
    marker_size=12;  
%     OASIS
    h = figure('visible','off')
%     scatter(parameters,OASIS,80,'b','filled')
    plot(parameters, OASIS,'r-o','LineWidth',linewidth,'markers',marker_size)
    xlabel('C', 'fontsize', labelsize)    
    ylabel('AP@10', 'fontsize', labelsize)
    ax = gca;
    ax.XTick = parameters2;    
    set(gca,'FontSize',20)
    grid
    set(gca, 'XScale', 'log')
    print(h,'-depsc',strcat(output_loc,dataset,'_par_PAAS.eps'));
    saveas(gcf,strcat(output_loc,dataset,'_par_PAAS'), 'pdf')
    close(h);  
   
    % AROW
    
%     X=zeros(num_par*num_par,1);
%     Y=zeros(num_par*num_par,1);
%     m = 1;
%     
%     for i = 1:num_par,
%         for j = 1:num_par,
%             X(m) = parameters(i);
%             Y(m) = parameters(j);
%             m = m + 1;
%         end
%     end
%     

    X = parameters; Y = parameters;
    Z = zeros(num_par, num_par);    
    for i = 1:num_par,        
        for j = 1:num_par,            
            Z(i,j) = AROW((i-1)*num_par+j);
        end
    end
    h = figure('visible','off')
%     h = figure
%     scatter3(X,Y,Z,70,'r')
%     mesh(X,Y,Z)
    surf(X,Y,Z)
    colorbar
    xlabel('\gamma', 'fontsize', labelsize)%, 'FontWeight','bold','Color', 'r')
    ylabel('\eta', 'fontsize', labelsize)%,'FontWeight','bold','Color', 'r')
%     zlabel('AP@10', 'fontsize', labelsize)
    ax = gca;
    ax.XTick = parameters3;    
    ax.YTick = parameters3;    
    set(gca, 'XScale', 'log')
    set(gca, 'YScale', 'log')
    set(gca,'FontSize',20)
    view([0,90])
    print(h,'-depsc',strcat(output_loc,dataset,'_par_CWAS.eps'));
    saveas(gcf,strcat(output_loc,dataset,'_par_CWAS'), 'pdf')
    close(h);  
  