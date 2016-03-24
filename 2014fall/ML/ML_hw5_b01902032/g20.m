[input_scale_label, input_scale_inst] = libsvmread('./inputz');

Eval = 1;
good_g = 1;
g_count = zeros(5,1);

for k = 1:100,
    gamma = 1;
    for j = 1:5,
    
        indexte = randperm(7291);
        indextr = indexte(1001:end);
        indexte(1001:end) = [];

        train_data = input_scale_inst(indextr(1:6291),:);    
        train_label = input_scale_label(indextr(1:6291),:);

        test_data = input_scale_inst(indexte(1:1000),:);
        test_label = input_scale_label(indexte(1:1000),:);


        str =  int2str(gamma);
        ostr = '-s 0 -t 2 -c 0.01 -g  ';
        strr = [ostr str];
        model = svmtrain(train_label,train_data, strr    ); 
        [predict_label, accuracy, dec_values] = svmpredict(test_label, test_data, model);
        if( accuracy(2) < Eval)
            Eval = accuracy(2);
            good_g = gamma;
        end;
        gamma = gamma *10;
    end;
    g_count(log10(good_g)+1) = g_count(log10(good_g)+1)+1;
    
end;
g_count
%w = model.SVs' * model.sv_coef;
%b = -model.rho;






