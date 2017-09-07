clear
dataset={'shuttle' 'segment' 'satimage' 'pendigits' 'letter' 'covtype'};% 'caltech249'}
inputLoc = '/home/snail/Copy/dataset/';
ouputLoc = '/home/snail/Dropbox/projects/research/AOS/dataset/';
num_folds = 5;
for d = 1:length(dataset),
    if strcmp(dataset{d},'shuttle'),        
        size_train_triplets = 4000;
    elseif strcmp(dataset{d},'segment'), 
        size_train_triplets = 25000;
    elseif strcmp(dataset{d},'satimage'), 
        size_train_triplets = 18000;
    elseif strcmp(dataset{d},'pendigits'), 
        size_train_triplets = 16848;    
    elseif strcmp(dataset{d},'letter'), 
        size_train_triplets = 10140;
    elseif strcmp(dataset{d},'covtype'), 
        size_train_triplets = 10000;
    elseif strcmp(dataset{d}, 'caltech249'),
        size_train_triplets = 300000;
    else
        size_train_triplets = 6000;
    end
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
    
    perm_idx = randperm(n1);
    Y = Y(perm_idx);
    X = X(perm_idx,:);
    
    ID_train=[];
    ID_test=[];
    triplets=[];
    
    num_folds = 5;
    train_test_indices = crossvalind('Kfold',n1,num_folds);
    
    norm_methods = {'length1' 'gaussian' 'scale01' 'orignal'};
    for idx_norm = 1:length(norm_methods)
        fprintf('%s normalization\n',norm_methods{idx_norm});
        if strcmp(norm_methods{idx_norm},'length1')% for each instance
           X_pro = normr(X);
        elseif strcmp(norm_methods{idx_norm},'gaussian')% for each feature
            for i=1:size(X,2)
                sd=std(X(:,i));
                if sd~=0.0,
                    mu=mean(X(:,i));
                    X_pro(:,i)=(X(:,i)-mu)/sd;
                end        
            end
        elseif strcmp(norm_methods{idx_norm},'scale01')
            for idx_fea = 1:size(X,2)
                fea_min = min(X(:,idx_fea));
                fea_max = max(X(:,idx_fea));
                X_pro(:,idx_fea)=(X(:,idx_fea)-fea_min)/(fea_max - fea_min);
            end;
        elseif strcmp(norm_methods{idx_norm},'orignal')
            X_pro = X;
        else
            error('Unknown normalization method');    
        end;
        X = X_pro;
        
        triplets = [];
        ID_test = [];
        ID_train = [];
        for j=1:num_folds,
            
            ID_test=[ID_test;find(train_test_indices==j)];
            train_idx = find(train_test_indices ~= j);
            ID_train=[ID_train;train_idx];      
            
            train_triplets=zeros(4,size_train_triplets);
            size_train=length(train_idx);
            
            for t=1:size_train_triplets, 
                trip=[];
                
                ind_x=train_idx(randi(size_train)); %\x_t
                ind_x_1=train_idx(randi(size_train));    

                while ind_x == ind_x_1,
                    ind_x_1 = train_idx(randi(size_train));
                end

                if Y(ind_x) == Y(ind_x_1),            
                    ind_x_2 = train_idx(randi(size_train));
                    while ind_x_2==ind_x || ind_x_2 == ind_x_1,
                        ind_x_2=train_idx(randi(size_train));
                    end           
                else
                    ind_x_2 = train_idx(randi(size_train));
                    while Y(ind_x) ~= Y(ind_x_2) || ind_x_2 == ind_x || ind_x_2 == ind_x_1,
                        ind_x_2=train_idx(randi(size_train));
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
    
            tripletsName=strcat(dataset{d},'_',norm_methods{idx_norm},'.triplets');
            dlmwrite(fullfile(ouputLoc,tripletsName),triplets',' ');

            trainID_file_name=strcat(dataset{d},'_',norm_methods{idx_norm},'.trainID');
            dlmwrite(fullfile(ouputLoc,trainID_file_name),ID_train);

            
            testID_file_name=strcat(dataset{d},'_',norm_methods{idx_norm},'.testID');
            dlmwrite(fullfile(ouputLoc,testID_file_name),ID_test);

            libsvmwrite(strcat(ouputLoc,dataset{d},'_',norm_methods{idx_norm}), Y, sparse(X));
    end;
end;
