% ///////////////////////////////////////////////////////
% Filename : buildComponentTree
% Description : build the component tree
% Author : Wang Kang
% Email : goto.champion@gmail.com
% Date : 2013/10/5
% ///////////////////////////////////////////////////////
function [nodes, adjs] = buildComponentTree(F)

[rows, cols] = size(F);

[~, idx] = sort(reshape(F, 1, rows*cols), 'descend');

if exist('buildComponentTreeCore.mexw32','file') == 0 && ...
        exist('buildComponentTreeCore.mexw64','file') == 0
    mex buildComponentTreeCore.cpp
    disp('Compile finished')
end

[nodes, adjs] = buildComponentTreeCore(F, int32(idx)-1);
