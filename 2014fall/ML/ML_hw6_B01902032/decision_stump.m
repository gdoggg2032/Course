function [s,dim,theta] = decision_stump(ins,SIZE,DIM,u)
    %[ins(:,1),ins(:,2),ins(:,3)] = textread('./hw6_train.dat.txt','%f %f %f');
    %SIZE = 100;
    %DIM = 2;

    
    Ein = sum(u);
    Rec = struct('theta',0,'dim',1,'s',1);
    Rec_num = 0;
    for k=1:DIM
        C = sortrows(ins,k);
        stmp =1;
        while(1)
            for i = 1:SIZE+1
                Eintmp = 0;
                if (i==1)
                    thetatmp = C(i,k)-0.001;
                elseif (i==SIZE+1)
                    thetatmp = C(i-1,k)+0.001;
                else
                    thetatmp = 0.5*(C(i-1,k)+C(i,k));
                end
                for j=1:SIZE
                    if(stmp*(ins(j,k)-thetatmp)*ins(j,DIM+1) <0)
                        
                        Eintmp = Eintmp + u(j);
                    end
                end
                if(Eintmp < Ein)
                    Rec_num =1;
                    Rec(Rec_num).theta = thetatmp;
                    Rec(Rec_num).dim=k;
                    Rec(Rec_num).s=stmp;
                    Ein = Eintmp;
                elseif(Eintmp==Ein)
                    Rec_num = Rec_num + 1;
                    Rec(Rec_num).theta = thetatmp;
                    Rec(Rec_num).dim=k;
                    Rec(Rec_num).s=stmp;
                    Ein = Eintmp;
                end

            end
            if(stmp==1)
                stmp=-1;
            else
                break;
            end
        end

    end
    Rec_num;
    I = 1;
    theta = Rec(I).theta;
    dim = Rec(I).dim;
    s = Rec(I).s;
    Er = 0;
    for i=1:SIZE
        predict_label(i,1) = s * sign(ins(i,dim)-theta);
        
    end
    
    
end
