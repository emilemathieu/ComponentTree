% ///////////////////////////////////////////////////////
% Filename : demo
% Description : run the demo to build the component tree
% Reference : L.Najman & M.Couprie - Building the component tree in quasi-linear time
% Author : Wang Kang
% Email : goto.champion@gmail.com
% Date : 2013/10/5
% ///////////////////////////////////////////////////////
clear all
clc

F = [110, 90, 100; ...     0  5  10
		50, 50, 50; ...      1   6  11
		40, 20, 50; ...      2  7  12
		50, 50, 50; ...      3  8  13
		120, 70, 80];   % 4  9  14
    
[nodes, adjs] = buildComponentTree(F);

plotAdjacentTree(nodes, adjs);
