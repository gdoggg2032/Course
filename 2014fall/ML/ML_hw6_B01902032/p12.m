
ins = zeros(100,3);
tins = zeros(1000,3);
%fd = fopen('./hw6_train.dat.txt');
%[ins(1:100,1) ins(1:100,2) ins(1:100,3)] = fscanf(fd,'%f %f %f',[100,3]);
%[ins(:,1),ins(:,2),ins(:,3)] = textread('./hw6_train.dat.txt','%f %f %f','headerline',0);
ins = load('./hw6_train.dat.txt');
%fdt = fopen('./hw6_test.dat.txt');
%[tins(:,1) tins(:,2) tins(:,3)] = fscanf(fdt,'%f %f %f',[1,1000]);
%[tins(:,1),tins(:,2),tins(:,3)] = textread('./hw6_test.dat.txt','%f %f %f','headerline',0);
tins = load('./hw6_test.dat.txt');
%fd = fopen(name)
%fscanf(fd, '%s', [1, 1000]

SIZE = 100;
DIM = 2;
%label = decision_stump(ins,SIZE,DIM,u)

N = 100;
T = 300;
u = zeros(N,T+1);
u(:,1) = ones(N,1) / N;
d = ones(T,1);
alpha = ones(T,1);
g = zeros(SIZE,T);
gout = zeros(1000,T);
G = zeros(SIZE,T);
Gout = zeros(1000,T);
U = zeros(T,1);
Eing=zeros(T,1);
Ein = zeros(T,1);
Eout = zeros(T,1);
for t = 1:T
    [s,dim,theta]  = decision_stump(ins,SIZE,DIM,u(:,t));
    for i =1:SIZE
         g(i,t) = s * sign(ins(i,dim)-theta);
         
    end
    for i=1:1000
        gout(i,t) = s * sign(tins(i,dim)-theta);
         
    end
    E = 0;
    er = 0;
    for i=1:SIZE
        if(g(i,t) ~= ins(i,3))
            E = E + u(i,t);
            er = er +1;
        end;
    end;
    %E
    %er
        E = E / sum(u(:,t));
        U(t) = sum(u(:,t));   
        d(t) = sqrt((1-E)/E);   
        alpha(t) = log(d(t));
    for i=1:SIZE
        if (g(i,t) * ins(i,3)>0)
            u(i,t+1) = u(i,t) / d(t);
        else
            Eing(t) = Eing(t) +1;
            u(i,t+1) = u(i,t) * d(t);
        end
        if(t==1)
            G(i,t) = g(i,t)*alpha(t);
        else
            G(i,t) = G(i,t-1) + g(i,t)*alpha(t);
        end
    end
    for(i=1:SIZE)
        if (G(i,t) * ins(i,3)<0)
            Ein(t) = Ein(t) +1;
        end
    end
        Ein(t) = Ein(t) / SIZE;
        Eing(t) = Eing(t) / SIZE ;
    for i =1:1000
        if(t==1)
            Gout(i,t) = gout(i,t)*alpha(t);
        else
            Gout(i,t) = Gout(i,t-1) + gout(i,t)*alpha(t);
        end
    end
    for(i=1:1000)
        if (Gout(i,t) * tins(i,3)<0)
            Eout(t) = Eout(t) +1;
        end
    end
    Eout(t) = Eout(t) / 1000;
    
    
end




Ein(T)

Eout(T)
U(T)
%G(x) = sign ??Tt =1 £\t gt (x)?



%???1 obtain gt by A(D, u(t)), where ...
%2 update u(t) to u(t+1) by ?t = ?1?£`t , where ...
%£`t
%?????3 compute £\t = ln(?t )
%return G(x) = sign ??Tt =1 £\t gt (x)?