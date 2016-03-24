Ein = zeros(5,1);
[input_scale_label, input_scale_inst] = libsvmread('./inputz');
[input2_scale_label, input2_scale_inst] = libsvmread('./inputz');

train_data = input_scale_inst(1:7291,:); 
train_label = input_scale_label(1:7291,:); 
test_data = input2_scale_inst(1:7291,:); 
test_label = input2_scale_label(1:7291,:); 
model = svmtrain(train_label,train_data, '-s 0 -t 1 -c 0.01 -g 1 -r 1 -d 2'    ); 
 
[predict_label, accuracy, dec_values] = svmpredict(test_label, test_data, model); % test the training data 

Ein(1) = accuracy(2);
[input_scale_label, input_scale_inst] = libsvmread('./inputtwo');
[input2_scale_label, input2_scale_inst] = libsvmread('./inputtwo');

train_data = input_scale_inst(1:7291,:); 
train_label = input_scale_label(1:7291,:); 
test_data = input2_scale_inst(1:7291,:); 
test_label = input2_scale_label(1:7291,:); 
model = svmtrain(train_label,train_data, '-s 0 -t 1 -c 0.01 -g 1 -r 1 -d 2'    ); 
 
[predict_label, accuracy, dec_values] = svmpredict(test_label, test_data, model); % test the training data 
Ein(2) = accuracy(2);

[input_scale_label, input_scale_inst] = libsvmread('./inputfour');
[input2_scale_label, input2_scale_inst] = libsvmread('./inputfour');

train_data = input_scale_inst(1:7291,:); 
train_label = input_scale_label(1:7291,:); 
test_data = input2_scale_inst(1:7291,:); 
test_label = input2_scale_label(1:7291,:); 
model = svmtrain(train_label,train_data, '-s 0 -t 1 -c 0.01 -g 1 -r 1 -d 2'    ); 
 
[predict_label, accuracy, dec_values] = svmpredict(test_label, test_data, model); % test the training data 
Ein(3) = accuracy(2);
[input_scale_label, input_scale_inst] = libsvmread('./inputsix');
[input2_scale_label, input2_scale_inst] = libsvmread('./inputsix');

train_data = input_scale_inst(1:7291,:); 
train_label = input_scale_label(1:7291,:); 
test_data = input2_scale_inst(1:7291,:); 
test_label = input2_scale_label(1:7291,:); 
model = svmtrain(train_label,train_data, '-s 0 -t 1 -c 0.01 -g 1 -r 1 -d 2'    ); 
 
[predict_label, accuracy, dec_values] = svmpredict(test_label, test_data, model); % test the training data 
Ein(4) = accuracy(2);
[input_scale_label, input_scale_inst] = libsvmread('./inputeight');
[input2_scale_label, input2_scale_inst] = libsvmread('./inputeight');

train_data = input_scale_inst(1:7291,:); 
train_label = input_scale_label(1:7291,:); 
test_data = input2_scale_inst(1:7291,:); 
test_label = input2_scale_label(1:7291,:); 
model = svmtrain(train_label,train_data, '-s 0 -t 1 -c 0.01 -g 1 -r 1 -d 2'    ); 
 
[predict_label, accuracy, dec_values] = svmpredict(test_label, test_data, model); % test the training data 
Ein(5) = accuracy(2);
Ein