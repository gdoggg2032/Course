[input_scale_label, input_scale_inst] = libsvmread('./inputz');
[input2_scale_label, input2_scale_inst] = libsvmread('./inputzz');

train_data = input_scale_inst(1:7291,:); 
train_label = input_scale_label(1:7291,:); 
test_data = input2_scale_inst(1:2007,:); 
test_label = input2_scale_label(1:2007,:); 
model = svmtrain(train_label,train_data, '-s 0 -t 2 -c 0.1 -g 1 '    ); 
 
[predict_label, accuracy, dec_values] = svmpredict(test_label, test_data, model); % test the training data 
 model = svmtrain(train_label,train_data, '-s 0 -t 2 -c 0.1 -g 10 '    ); 
 
[predict_label, accuracy, dec_values] = svmpredict(test_label, test_data, model); % test the training data 
 

model = svmtrain(train_label,train_data, '-s 0 -t 2 -c 0.1 -g 100 '    ); 
 
[predict_label, accuracy, dec_values] = svmpredict(test_label, test_data, model); % test the training data 
 

model = svmtrain(train_label,train_data, '-s 0 -t 2 -c 0.1 -g 1000 '    ); 
 
[predict_label, accuracy, dec_values] = svmpredict(test_label, test_data, model); % test the training data 
 

model = svmtrain(train_label,train_data, '-s 0 -t 2 -c 0.1 -g 10000 '    ); 
 
[predict_label, accuracy, dec_values] = svmpredict(test_label, test_data, model); % test the training data 
 








