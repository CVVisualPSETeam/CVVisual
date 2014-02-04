---
layout: page
title: "Filter query language"
category: doc
date: 2014-02-04 08:55:36
order: 1
---
The filter query language is the query language used in the overview and the raw match view to simply the task of filtering, sorting and grouping data sets in a table UI.
The following is a description of the simple syntax and the supported commands.

Just type `#` into the search field to see some supported commands, using the suggestions feature (it's inspired by the awesome z shell).

##Syntax
A query consist basically of many subqueries starting with a `#`:
```
[raw filter subquery] #[subquery 1] [...] #[subquery n]
```
The optional first part of the query doesn't start with a `#`, it's short for `#raw [...]`.


There three different types of subqueries:

###Sort query
A sort query has the following structure:
```
sort by [sort subquery 1], [...], [sort subquery n]
```

A sort subquery consist of a sort command (aka "the feature by which you want to sort the table") and a sort order:
- `[command]`: equivalent to `[command] asc`
- `[command] asc`: sorts in ascending order
- `[command] desc`: sorts in descending order

(The sort command is typically a single word.)

For your interest: The `[subquery n]` has higher priority than the `[subquery n+1]`. 

###Group query
A group query has the following structure:
```
group by [command 1], [...], [command n]
```
A group command is a single word declaring the feature you want to group the data sets in the table by.
The group header consist of the `n` items.

For your interest: The raw view currently doesn't support group queries.

###Filter query
A filter query is the basic type of query, allowing you to filter the data sets by several criterias.
It has the following structure:
```
#[filter command] [argument]
```
It also supports several arguments for one filter command (via the comma seperated filters feature):
```
#[cs filter command] [argument 1], [...], [argument n]
```

##Overview
The following commands are supported in the overview.

feauture/command | sorting supported | grouping supported | filtering supported | description
-----------------|-------------------|--------------------|--------------------------------------------
id               | yes               | yes                | yes                 |
raw              | yes               | yes                | only basic filter   | alias for description
description      | yes               | yes                | only basic filter   |
image_count      | yes               | yes                | yes                 | number of images
function         | yes               | yes                | yes                 | calling function
file             | yes               | yes                | yes                 | inheriting file
line             | yes               | yes                | yes                 |
type             | yes               | yes                | yes                 | call type   
            

##TODO
Add raw match view paragraphs... 
