% ///////////////////////////////////////////////////////
% Filename : plotAdjacentTree
% Description : plot the tree in adjacent structure
% Author : Wang Kang
% Email : goto.champion@gmail.com
% Date : 2013/10/5
% ///////////////////////////////////////////////////////
function plotAdjacentTree(nodes, adjs)

adjLabels = num2str(nodes);
adjNum = size(nodes, 1);

adjMap = zeros(adjNum, adjNum);
for i = 1 : size(adjs, 1)
    row = find(nodes == adjs(i,1));
    col = find(nodes == adjs(i,2));
    adjMap(row, col) = 1;
end

bg = biograph(adjMap, adjLabels);
view(bg)