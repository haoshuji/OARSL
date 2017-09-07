clear
dataset = {'shuttle_scale'};
inputLoc = 'D:\dataset\';
ouputLoc = '.\data\';
for d = 1:length(dataset),
    display(dataset{d});
    [Y,X]=libsvmread(strcat(inputLoc,dataset{d}));
    [n1,d1] = size(Y);
    [n2,d2] = size(X);
    if n1 == n2,
        fprintf('Number of instances is:%d\n',n1);
        fprintf('Number of features is:%d\n',d2);
    else
        fprintf('Failed to load data: %s',dataset{d});
    end
    %% transfer to column vector
  
    ID_train=[];
    ID_test=[];
    triplets=[];
    
    [Y,I]=sort(Y,'ascend');
    X=X(I,:);
    uniq=unique(Y);
    fprintf('Number of classes is:%d\n',length(uniq));
    num_ins_each_class=[];
    for i=1:length(uniq),
        num_ins_each_class(i) = sum(Y(:)==uniq(i));
        test_instances(i)=floor(num_ins_each_class(i)*0.2);
    end
    
%     Y_tmp=[];
%     X_tmp=[];
%     start_ind = 1;
%     for i=1:length(uniq),
%         if num_ins_each_class(i) < 100,
%             Y_tmp=[Y_tmp;Y(start_ind:(start_ind+num_ins_each_class(i)-1))];
%             X_tmp=[X_tmp;X(start_ind:(start_ind+num_ins_each_class(i)-1),:)];
%         else
%             Y_tmp=[Y_tmp;Y(start_ind:(start_ind+100))];
%             X_tmp=[X_tmp;X(start_ind:(start_ind+100),:)];
%         end
%         start_ind=start_ind+num_ins_each_class(i);
%     end
%     
%     X=X_tmp;
%     Y=Y_tmp;
    
    fprintf('Number of instances is:%d\n',size(Y,1));     
    for i=1:length(uniq),
        num_ins_each_class(i) = sum(Y(:)==uniq(i));
        test_instances(i)=floor(num_ins_each_class(i)*0.2);
    end

    size_train_triplets = 4000;
    
    for j=1:5,
        train=[];
        test=[];
        tmp=0;
        for i=1:length(uniq),
            test = [test, tmp+(((j-1)*test_instances(i)+1):((j*test_instances(i))))];
            train = [train,tmp + ( 1:((j-1)*test_instances(i)))];
            train = [train,tmp + ((j*test_instances(i)+1):num_ins_each_class(i))];
            tmp=tmp+num_ins_each_class(i);
        end

        ID_test=[ID_test,test'];
        ID_train=[ID_train,train'];      
        train_triplets=zeros(4,size_train_triplets);
        size_train=size(train,2);
        for t=1:size_train_triplets, 
            trip=[];
            %% positive triplet
            ind_x=train(randi(size_train)); %\x_t
            ind_x_1=train(randi(size_train));    
           
            while ind_x == ind_x_1,
                ind_x_1 = train(randi(size_train));
            end
            
            if Y(ind_x) == Y(ind_x_1),            
                ind_x_2 = train(randi(size_train));
                while ind_x_2==ind_x || ind_x_2 == ind_x_1,
                    ind_x_2=train(randi(size_train));
                end           
            else
                ind_x_2 = train(randi(size_train));
                while Y(ind_x) ~= Y(ind_x_2) || ind_x_2 == ind_x || ind_x_2 == ind_x_1,
                    ind_x_2=train(randi(size_train));
                end                  
                tmp=ind_x_2;
                ind_x_2=ind_x_1;
                ind_x_1=tmp;
            end
            
            if rand>0.5,
                tri=[1;ind_x;ind_x_1;ind_x_2];
            else
                tri=[-1;ind_x;ind_x_2;ind_x_1];
            end
            
            train_triplets(:,t)=tri; %col
        end
        triplets=[triplets,train_triplets];   
    end
%         save(strcat('../',dataset{d},'.mat'),'X', 'Y', 'triplets', 'ID_train', 'ID_test');        
        
        tripletsName=strcat(dataset{d},'.triplets');
        dlmwrite(fullfile(ouputLoc,tripletsName),triplets',' ');
        
        trainTmp=ID_train(:);%reshape(ID_train',1,size(ID_train,2)*size(ID_train,1));
        
        trainID=strcat(dataset{d},'.trainID');
        dlmwrite(fullfile(ouputLoc,trainID),trainTmp,' ');
        
        testTmp = ID_test(:);%reshape(ID_test',1,size(ID_test,2)*size(ID_test,1));
        testID=strcat(dataset{d},'.testID');
        dlmwrite(fullfile(ouputLoc,testID),testTmp,' ');

        libsvmwrite(strcat(ouputLoc,dataset{d}), Y, sparse(X));
end