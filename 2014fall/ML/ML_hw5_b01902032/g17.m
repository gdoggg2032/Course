
sig = zeros(5,1);

[input_scale_label, input_scale_inst] = libsvmread('./inputz');
[input2_scale_label, input2_scale_inst] = libsvmread('./inputz');

train_data = input_scale_inst(1:7291,:); 
train_label = input_scale_label(1:7291,:); 
test_data = input2_scale_inst(1:7291,:); 
test_label = input2_scale_label(1:7291,:); 
model = svmtrain(train_label,train_data, '-s 0 -t 1 -c 0.01 -g 1 -r 1 -d 2'    ); 
 
SV = sum(model.nSV);
SVs = model.SVs;
[predict_label, accuracy, dec_values] = svmpredict(test_label, test_data, model); % test the training data 
z = [ ones(SV,1) sqrt(2)*SVs  SVs(1:SV,1).^2 SVs(1:SV,1).*SVs(1:SV,2) SVs(1:SV,2).^2 ];
w =z' * model.sv_coef;
sig(1) = 0.5*norm(w)^2 + 19.976820;

[input_scale_label, input_scale_inst] = libsvmread('./inputtwo');
[input2_scale_label, input2_scale_inst] = libsvmread('./inputtwo');

train_data = input_scale_inst(1:7291,:); 
train_label = input_scale_label(1:7291,:); 
test_data = input2_scale_inst(1:7291,:); 
test_label = input2_scale_label(1:7291,:); 
model = svmtrain(train_label,train_data, '-s 0 -t 1 -c 0.01 -g 1 -r 1 -d 2'    ); 
 
SV = sum(model.nSV);
SVs = model.SVs;
[predict_label, accuracy, dec_values] = svmpredict(test_label, test_data, model); % test the training data 
z = [ ones(SV,1) sqrt(2)*SVs  SVs(1:SV,1).^2 SVs(1:SV,1).*SVs(1:SV,2) SVs(1:SV,2).^2 ];
w =z' * model.sv_coef;
sig(2) = 0.5*norm(w)^2 + +14.620013;

[input_scale_label, input_scale_inst] = libsvmread('./inputfour');
[input2_scale_label, input2_scale_inst] = libsvmread('./inputfour');

train_data = input_scale_inst(1:7291,:); 
train_label = input_scale_label(1:7291,:); 
test_data = input2_scale_inst(1:7291,:); 
test_label = input2_scale_label(1:7291,:); 
model = svmtrain(train_label,train_data, '-s 0 -t 1 -c 0.01 -g 1 -r 1 -d 2'    ); 
 
SV = sum(model.nSV);
SVs = model.SVs;
[predict_label, accuracy, dec_values] = svmpredict(test_label, test_data, model); % test the training data 
z = [ ones(SV,1) sqrt(2)*SVs  SVs(1:SV,1).^2 SVs(1:SV,1).*SVs(1:SV,2) SVs(1:SV,2).^2 ];
w =z' * model.sv_coef;
sig(3) = 0.5*norm(w)^2 + +13.040000;

[input_scale_label, input_scale_inst] = libsvmread('./inputsix');
[input2_scale_label, input2_scale_inst] = libsvmread('./inputsix');

train_data = input_scale_inst(1:7291,:); 
train_label = input_scale_label(1:7291,:); 
test_data = input2_scale_inst(1:7291,:); 
test_label = input2_scale_label(1:7291,:); 
model = svmtrain(train_label,train_data, '-s 0 -t 1 -c 0.01 -g 1 -r 1 -d 2'    ); 
 
SV = sum(model.nSV);
SVs = model.SVs;
[predict_label, accuracy, dec_values] = svmpredict(test_label, test_data, model); % test the training data 
z = [ ones(SV,1) sqrt(2)*SVs  SVs(1:SV,1).^2 SVs(1:SV,1).*SVs(1:SV,2) SVs(1:SV,2).^2 ];
w =z' * model.sv_coef;
sig(4) = 0.5*norm(w)^2 + +13.279999;

[input_scale_label, input_scale_inst] = libsvmread('./inputeight');
[input2_scale_label, input2_scale_inst] = libsvmread('./inputeight');

train_data = input_scale_inst(1:7291,:); 
train_label = input_scale_label(1:7291,:); 
test_data = input2_scale_inst(1:7291,:); 
test_label = input2_scale_label(1:7291,:); 
model = svmtrain(train_label,train_data, '-s 0 -t 1 -c 0.01 -g 1 -r 1 -d 2'    ); 
 
SV = sum(model.nSV);
SVs = model.SVs;
[predict_label, accuracy, dec_values] = svmpredict(test_label, test_data, model); % test the training data 
z = [ ones(SV,1) sqrt(2)*SVs  SVs(1:SV,1).^2 SVs(1:SV,1).*SVs(1:SV,2) SVs(1:SV,2).^2 ];
w =z' * model.sv_coef;
sig(5) = 0.5*norm(w)^2 + +10.839995;

sig







