O1 = zeros(5,1);
O2 = zeros(5,1);
O3 = zeros(5,1);
O4 = zeros(5,1);
O5 = zeros(5,1);

SV_dis = zeros(7291,1);

[input_scale_label, input_scale_inst] = libsvmread('./inputz');
[input2_scale_label, input2_scale_inst] = libsvmread('./inputzz');

train_data = input_scale_inst(1:7291,:); 
train_label = input_scale_label(1:7291,:); 
test_data = input2_scale_inst(1:2007,:); 
test_label = input2_scale_label(1:2007,:); 

model = svmtrain(train_label,train_data, '-s 0 -t 2 -c 0.001 -g 100 '    ); 
 
[predict_label, accuracy, dec_values] = svmpredict(test_label, test_data, model); % test the training data 
A = zeros(model.totalSV,model.totalSV);

K = zeros(model.totalSV,model.totalSV);

B_label = zeros(model.totalSV,1);





for i = 1:model.totalSV,
    for j =1:model.totalSV,
        K(i,j) = exp(-100*norm(model.SVs(i,:)-model.SVs(j,:))^2);
        A(i,j) = model.sv_coef(i) * model.sv_coef(j);
    end;
end;

S = A.*K;
SS = sum(S(:)); %||w||^2

O1(1) = 1/sqrt(SS);
O2(1) = (-0.5*SS +2.380630)/0.001;
O3(1) = model.totalSV;
O4(1) = accuracy(2);
O5(1) = -2.380630;

model = svmtrain(train_label,train_data, '-s 0 -t 2 -c 0.01 -g 100 '    ); 
 
[predict_label, accuracy, dec_values] = svmpredict(test_label, test_data, model); % test the training data 
A = zeros(model.totalSV,model.totalSV);

K = zeros(model.totalSV,model.totalSV);

B_label = zeros(model.totalSV,1);





for i = 1:model.totalSV,
    for j =1:model.totalSV,
        K(i,j) = exp(-100*norm(model.SVs(i,:)-model.SVs(j,:))^2);
        A(i,j) = model.sv_coef(i) * model.sv_coef(j);
    end;
end;

S = A.*K;
SS = sum(S(:)); %||w||^2

O1(2) = 1/sqrt(SS);
O2(2) = (-0.5*SS +23.145040)/0.01;
O3(2) = model.totalSV;
O4(2) = accuracy(2);
O5(2) = -23.145040;



model = svmtrain(train_label,train_data, '-s 0 -t 2 -c 0.1 -g 100 '    ); 
 
[predict_label, accuracy, dec_values] = svmpredict(test_label, test_data, model); % test the training data 
 
A = zeros(model.totalSV,model.totalSV);

K = zeros(model.totalSV,model.totalSV);

B_label = zeros(model.totalSV,1);





for i = 1:model.totalSV,
    for j =1:model.totalSV,
        K(i,j) = exp(-100*norm(model.SVs(i,:)-model.SVs(j,:))^2);
        A(i,j) = model.sv_coef(i) * model.sv_coef(j);
    end;
end;

S = A.*K;
SS = sum(S(:)); %||w||^2

O1(3) = 1/sqrt(SS);
O2(3) = (-0.5*SS +178.198722)/0.1;
O3(3) = model.totalSV;
O4(3) = accuracy(2);
O5(3) = -178.198722;
model = svmtrain(train_label,train_data, '-s 0 -t 2 -c 1 -g 100 '    ); 
 
[predict_label, accuracy, dec_values] = svmpredict(test_label, test_data, model); % test the training data 
 
A = zeros(model.totalSV,model.totalSV);

K = zeros(model.totalSV,model.totalSV);

B_label = zeros(model.totalSV,1);





for i = 1:model.totalSV,
    for j =1:model.totalSV,
        K(i,j) = exp(-100*norm(model.SVs(i,:)-model.SVs(j,:))^2);
        A(i,j) = model.sv_coef(i) * model.sv_coef(j);
    end;
end;

S = A.*K;
SS = sum(S(:)); %||w||^2
O1(4) = 1/sqrt(SS);
O2(4) = (-0.5*SS +1401.259572)/1;
O3(4) = model.totalSV;
O4(4) = accuracy(2);
O5(4) = -1401.259572;
model = svmtrain(train_label,train_data, '-s 0 -t 2 -c 10 -g 100 '    ); 
 
[predict_label, accuracy, dec_values] = svmpredict(test_label, test_data, model); % test the training data 
 
A = zeros(model.totalSV,model.totalSV);

K = zeros(model.totalSV,model.totalSV);

B_label = zeros(model.totalSV,1);





for i = 1:model.totalSV,
    for j =1:model.totalSV,
        K(i,j) = exp(-100*norm(model.SVs(i,:)-model.SVs(j,:))^2);
        A(i,j) = model.sv_coef(i) * model.sv_coef(j);
    end;
end;

S = A.*K;
SS = sum(S(:)); %||w||^2
O1(5) = 1/sqrt(SS);
O2(5) = (-0.5*SS +13027.301309)/10;
O3(5) = model.totalSV;
O4(5) = accuracy(2);
O5(5) = -13027.301309;

O1
O2
O3
O4
O5

