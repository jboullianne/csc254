-- Jean-Marc Boullianne
-- jboullia@u.rochester.edu
-- CSC 254: Fall 2015
-- A1: Tree Enumeration

-- Import resources 
with Ada.Text_IO;
with Ada.Strings.Unbounded;
with Ada.Command_Line;

-- Tell program which resource we are using
use Ada.Strings.Unbounded;
use Ada.Text_IO;


-- Recursively Enumerates Trees
procedure TreeEnumeration is

  procedure Build_tree (NLeft, Depth : Integer;
  Tree : Ada.Strings.Unbounded.Unbounded_String) is
  
  -- Unbounded String Variables to hold enumerated trees
  NewTree: Ada.Strings.Unbounded.Unbounded_String := Tree;
  TempTree : Ada.Strings.Unbounded.Unbounded_String := Tree;
  
  -- Decides whether to move up or add node to tree
  begin
    if NLeft = 0 and then Depth = 0 then
        Put_Line(Ada.Strings.Unbounded.To_String(NewTree));
    else if NLeft = 0 and then Depth /= 0 then
        Append(NewTree, To_Unbounded_String(")"));
        Build_Tree(NLeft, Depth -1, NewTree);
    else
        Append(NewTree, To_Unbounded_String("("));
        Build_Tree(NLeft - 1, Depth + 1, NewTree);
        if Depth > 1 then
            Append(TempTree, To_Unbounded_String(")"));
            Build_Tree(NLeft, Depth -1, TempTree);
        end if;
    end if;
    end if;
  end Build_tree;
  
-- Starts tree building procedure with <Number of Nodes> passed in as command line argument
begin
    Build_tree (Integer'Value(Ada.Command_Line.Argument(1)) , 0, Ada.Strings.Unbounded.To_Unbounded_String(""));
end TreeEnumeration;
