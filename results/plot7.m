function plot7(loc,dataset)

	M=dlmread(strcat(loc,dataset));    
    n=1;
    
%      num = 57; 6
    num=6;
%      precsion
    A=[];
    B=[];
    for i=1:7,
        A=[A;M((i-1)*num+1,:)];
        A=[A;M((i-1)*num+2,:)];
        A=[A;M((i-1)*num+3,:)];
        
        B=[B;M((i-1)*num+1,:)];
        B=[B;M((i-1)*num+4,:)];
        B=[B;M((i-1)*num+5,:)];        
    end
    
    legendsize=10;
    labelsize=14;
    linewidth=2;
    linemaker=8;    
    

    h=figure('visible','off');
    plot(A(1,:),A(2,:),'m-o','LineWidth',linewidth,'markers',linemaker);
    hold on
    plot(A(4,:),A(5,:),'b-o','LineWidth',linewidth,'markers',linemaker);        
    plot(A(7,:),A(8,:),'c-o','LineWidth',linewidth,'markers',linemaker); 
    
    plot(A(10,:),A(11,:),'m->','LineWidth',linewidth,'markers',linemaker);
    plot(A(13,:),A(14,:),'b->','LineWidth',linewidth,'markers',linemaker);  
    plot(A(16,:),A(17,:),'c->','LineWidth',linewidth,'markers',linemaker);  
    
    plot(A(19,:),A(20,:),'k-o','LineWidth',linewidth,'markers',linemaker);
    legend('FAOSL','FROSL', 'FOSL',  'SAOSL','SROSL','SOSL', 'OASIS', 'fontsize',legendsize,'Location','Best'); 
    XLABEL('Ratio of Query', 'fontsize', labelsize);
    YLABEL('Top-10-Precision','fontsize', labelsize)
    grid
    print(h,'-depsc',strcat(loc,dataset,'Top10PreTrain.eps'));
    close(h);    
    
    h=figure('visible','off');
    plot(A(1,:),A(3,:),'m-o','LineWidth',linewidth,'markers',linemaker);
    hold on
    plot(A(4,:),A(6,:),'b-o','LineWidth',linewidth,'markers',linemaker);        
    plot(A(7,:),A(9,:),'c-o','LineWidth',linewidth,'markers',linemaker); 
    
    plot(A(10,:),A(12,:),'m->','LineWidth',linewidth,'markers',linemaker);
    plot(A(13,:),A(15,:),'b->','LineWidth',linewidth,'markers',linemaker);  
     plot(A(16,:),A(18,:),'c->','LineWidth',linewidth,'markers',linemaker);  
    
     plot(A(19,:),A(20,:),'k-o','LineWidth',linewidth,'markers',linemaker);
     legend('FAOSL','FROSL', 'FOSL',  'SAOSL','SROSL','SOSL', 'OASIS', 'fontsize',legendsize,'Location','Best'); 
    XLABEL('Ratio of Query', 'fontsize', labelsize);
    YLABEL('Average Top-10-Precision','fontsize', labelsize)
    grid
    print(h,'-depsc',strcat(loc,dataset,'Top10AvgPreTrain.eps'));
    close(h);      
    
    A = log(A);
    h=figure('visible','off');
    plot(A(1,:),A(2,:),'m-o','LineWidth',linewidth,'markers',linemaker);
    hold on
    plot(A(4,:),A(5,:),'b-o','LineWidth',linewidth,'markers',linemaker);        
     plot(A(7,:),A(8,:),'c-o','LineWidth',linewidth,'markers',linemaker); 
    
    plot(A(10,:),A(11,:),'m->','LineWidth',linewidth,'markers',linemaker);
    plot(A(13,:),A(14,:),'b->','LineWidth',linewidth,'markers',linemaker);  
     plot(A(16,:),A(17,:),'c->','LineWidth',linewidth,'markers',linemaker);  
    
     plot(A(19,:),A(20,:),'k-o','LineWidth',linewidth,'markers',linemaker);
     legend('FAOSL','FROSL', 'FOSL',  'SAOSL','SROSL','SOSL', 'OASIS', 'fontsize',legendsize,'Location','Best'); 
    XLABEL('Ratio of Query', 'fontsize', labelsize);
    YLABEL('Top-10-Precision','fontsize', labelsize)
    grid
    print(h,'-depsc',strcat(loc,dataset,'Top10PreTrainLog.eps'));
    close(h);    
    
    h=figure('visible','off');
    plot(A(1,:),A(3,:),'m-o','LineWidth',linewidth,'markers',linemaker);
    hold on
    plot(A(4,:),A(6,:),'b-o','LineWidth',linewidth,'markers',linemaker);        
     plot(A(7,:),A(9,:),'c-o','LineWidth',linewidth,'markers',linemaker); 
    
    plot(A(10,:),A(12,:),'m->','LineWidth',linewidth,'markers',linemaker);
    plot(A(13,:),A(15,:),'b->','LineWidth',linewidth,'markers',linemaker);  
     plot(A(16,:),A(18,:),'c->','LineWidth',linewidth,'markers',linemaker);  
    
     plot(A(19,:),A(20,:),'k-o','LineWidth',linewidth,'markers',linemaker);
     legend('FAOSL','FROSL', 'FOSL',  'SAOSL','SROSL','SOSL', 'OASIS', 'fontsize',legendsize,'Location','Best'); 
    XLABEL('Ratio of Query', 'fontsize', labelsize);
    YLABEL('Average Top-10-Precision','fontsize', labelsize)
    grid
    print(h,'-depsc',strcat(loc,dataset,'Top10AvgPreTrainLog.eps'));
    close(h);     
    
    A=B;
    h=figure('visible','off');
    plot(A(1,:),A(2,:),'m-o','LineWidth',linewidth,'markers',linemaker);
    hold on
    plot(A(4,:),A(5,:),'b-o','LineWidth',linewidth,'markers',linemaker);        
     plot(A(7,:),A(8,:),'c-o','LineWidth',linewidth,'markers',linemaker); 
     
    plot(A(10,:),A(11,:),'m->','LineWidth',linewidth,'markers',linemaker);
    plot(A(13,:),A(14,:),'b->','LineWidth',linewidth,'markers',linemaker);  
     plot(A(16,:),A(17,:),'c->','LineWidth',linewidth,'markers',linemaker);  
    
     plot(A(19,:),A(20,:),'k-o','LineWidth',linewidth,'markers',linemaker);
     legend('FAOSL','FROSL', 'FOSL',  'SAOSL','SROSL','SOSL', 'OASIS', 'fontsize',legendsize,'Location','Best'); 
    XLABEL('Ratio of Query', 'fontsize', labelsize);
    YLABEL('Top-10-Precision','fontsize', labelsize)
    grid
    print(h,'-depsc',strcat(loc,dataset,'Top10PreTest.eps'));
    close(h);    
    
    h=figure('visible','off');
    plot(A(1,:),A(3,:),'m-o','LineWidth',linewidth,'markers',linemaker);
    hold on
    plot(A(4,:),A(6,:),'b-o','LineWidth',linewidth,'markers',linemaker);        
     plot(A(7,:),A(9,:),'c-o','LineWidth',linewidth,'markers',linemaker); 
    
    plot(A(10,:),A(12,:),'m->','LineWidth',linewidth,'markers',linemaker);
    plot(A(13,:),A(15,:),'b->','LineWidth',linewidth,'markers',linemaker);  
     plot(A(16,:),A(18,:),'c->','LineWidth',linewidth,'markers',linemaker);  
    
     plot(A(19,:),A(20,:),'k-o','LineWidth',linewidth,'markers',linemaker);
     legend('FAOSL','FROSL', 'FOSL',  'SAOSL','SROSL','SOSL', 'OASIS', 'fontsize',legendsize,'Location','Best');  
    XLABEL('Ratio of Query', 'fontsize', labelsize);
    YLABEL('Average Top-10-Precision','fontsize', labelsize)
    grid
    print(h,'-depsc',strcat(loc,dataset,'Top10AvgPreTest.eps'));
    close(h);      
   
    
    A = log(A);
    h=figure('visible','off');
    plot(A(1,:),A(2,:),'m-o','LineWidth',linewidth,'markers',linemaker);
    hold on
    plot(A(4,:),A(5,:),'b-o','LineWidth',linewidth,'markers',linemaker);        
     plot(A(7,:),A(8,:),'c-o','LineWidth',linewidth,'markers',linemaker); 
    
    plot(A(10,:),A(11,:),'m->','LineWidth',linewidth,'markers',linemaker);
    plot(A(13,:),A(14,:),'b->','LineWidth',linewidth,'markers',linemaker);  
     plot(A(16,:),A(17,:),'c->','LineWidth',linewidth,'markers',linemaker);  
    
     plot(A(19,:),A(20,:),'k-o','LineWidth',linewidth,'markers',linemaker);
    legend('FAOSL','FROSL', 'FOSL',  'SAOSL','SROSL','SOSL', 'OASIS', 'fontsize',legendsize,'Location','Best'); 
    XLABEL('Ratio of Query', 'fontsize', labelsize);
    YLABEL('Top-10-Precision','fontsize', labelsize)
    grid
    print(h,'-depsc',strcat(loc,dataset,'Top10PreTestLog.eps'));
    close(h);    
    
    h=figure('visible','off');
    plot(A(1,:),A(3,:),'m-o','LineWidth',linewidth,'markers',linemaker);
    hold on
    plot(A(4,:),A(6,:),'b-o','LineWidth',linewidth,'markers',linemaker);        
     plot(A(7,:),A(9,:),'c-o','LineWidth',linewidth,'markers',linemaker); 
    
    plot(A(10,:),A(12,:),'m->','LineWidth',linewidth,'markers',linemaker);
    plot(A(13,:),A(15,:),'b->','LineWidth',linewidth,'markers',linemaker);  
     plot(A(16,:),A(18,:),'c->','LineWidth',linewidth,'markers',linemaker);  
    
     plot(A(19,:),A(20,:),'k-o','LineWidth',linewidth,'markers',linemaker);
    legend('FAOSL','FROSL', 'FOSL',  'SAOSL','SROSL','SOSL', 'OASIS', 'fontsize',legendsize,'Location','Best'); 
    XLABEL('Ratio of Query', 'fontsize', labelsize);
    YLABEL('Average Top-10-Precision','fontsize', labelsize)
    grid
    print(h,'-depsc',strcat(loc,dataset,'Top10AvgPreTestLog.eps'));
    close(h);